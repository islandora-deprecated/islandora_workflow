<?php
/**
 * @file
 * Provides Rules events and an action for integration with workflow.
 */

define('ISLANDORA_RULES_MANAGER', ISLANDORA_WORKFLOW_MANAGER);
define('ISLANDORA_RULES_EDITOR', ISLANDORA_WORKFLOW_EDITOR);
define('ISLANDORA_RULES_SUBMITTER', ISLANDORA_WORKFLOW_SUBMITTER);
define('ISLANDORA_RULES_ADMINISTRATOR', ISLANDORA_WORKFLOW_ADMIN);

/**
 * Implements hook_rules_action_info().
 */
function islandora_workflow_rules_action_info() {
  return array(
    'islandora_workflow_rules_email_someone' => array(
      'label' => t('Email Someone'),
      'module' => 'Islandora Rules',
      'arguments' => array(
        'object' => array(
          'type' => 'fedora_object',
          'label' => t('The object that has been modified'),
        ),
      ),
      'eval input' => array('message'),
    ),
  );
}

/**
 * Get a list of recipients for a email notification on an object change.
 *
 * @param Fedora_Item $object
 *   a Fedora object
 * @param array $settings
 *   an array of Rules settings
 *
 * @return array
 *   an array of Drupal user ids
 */
function islandora_workflow_rules_email_recipients($object, $settings) {
  module_load_include('inc', 'fedora_repository', 'api/fedora_item');
  module_load_include('permissions.inc', 'islandora_workflow');
  module_load_include('inc', 'islandora_workflow');

  // Variable in which to accumulate the array of recipients. (user ids)
  $recipients = array();

  // Get everyone who has the given workflow roles for the parent collection.
  if ($settings['roles']) {
    $collections = array();
    $parents = $object->get_relationships('fedora:isMemberOf');
    foreach ($parents as $parent) {
      $collections[] = $parent['object'];
    }
    $parents = $object->get_relationships('fedora:isMemberOfCollection');
    foreach ($parents as $parent) {
      $collections[] = $parent['object'];
    }
    $options = array(
      'collection' => $collections,
      'permissions' => $settings['roles'],
    );

    $user_permissions = islandora_workflow_get_user_permissions($options);

    // Return an array with the keys [user,collection,permission].
    foreach ($user_permissions as $user_permission) {
      $uid = $user_permission['user'];
      $recipients[] = $uid;
    }
  }

  if ($settings['notify_creator']) {
    $creator_name = islandora_workflow_get_object_creator($object->pid);
    $creator = user_load(array('name' => $creator_name));
    if ($creator) {
      $recipients[] = $creator->uid;
    }
  }
  if ($settings['notify_assignee']) {
    $assignee_name = islandora_workflow_get_object_assignee($object->pid);
    $assignee = user_load(array('name' => $assignee_name));
    if ($assignee) {
      $recipients[] = $assignee->uid;
    }
  }
  return array_filter(array_unique($recipients));
}

/**
 * A Rules action to send email to people.
 *
 * @param Fedora_Item $object
 *   The object that has been altered
 * @param array $settings
 *   An array of Rules settings
 */
function islandora_workflow_rules_email_someone($object, $settings) {
  $recipients = islandora_workflow_rules_email_recipients($object, $settings);
  foreach ($recipients as $recipient) {
    $account = user_load($recipient);
    // TODO:  Find some way to make the language actually send the message in
    // the preferred language of the user.
    drupal_mail('islandora_workflow', NULL, $account->mail, user_preferred_language($account), $settings);
    watchdog('iw_rules', 'Sending message about %object to %address', array(
      '%object' => $object->pid,
      '%address' => $account->mail,
    ));
  }
}

/**
 * Form definition.
 *
 * Settings form for the Rules action to send people email.
 *
 * @param array $settings
 *   An array of existing settings
 * @param array $form
 *   The settings form to modify
 */
function islandora_workflow_rules_email_someone_form($settings, &$form) {
  $settings += array(
    'subject' => '',
    'html_message' => TRUE,
    'message' => '',
    'roles' => array('admin'),
    'notify_creator' => TRUE,
    'notify_assignee' => FALSE,
  );
  $form['settings']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message subject'),
    '#default_value' => $settings['subject'],
    '#description' => t('The subject to set on the sent message.'),
  );
  $form['settings']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
    '#description' => t('The message to be sent.'),
  );
  $form['settings']['html_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send as HTML message'),
    '#default_value' => $settings['html_message'],
    '#description' => t('Adjusts the e-mail headers so HTML tags should be properly used.'),
  );
  $role_options = array(
    ISLANDORA_RULES_ADMINISTRATOR => t('Workflow Administrator'),
    ISLANDORA_RULES_MANAGER => t('Manager'),
    ISLANDORA_RULES_EDITOR => t('Editor'),
  );
  $form['settings']['roles'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Workflow Role'),
    '#description' => t('Select the roles to be notified.'),
    '#options' => $role_options,
    '#default_value' => $settings['roles'],
  );
  $form['settings']['notify_creator'] = array(
    '#type' => 'checkbox',
    '#title' => t("Notify the object's creator"),
    '#default_value' => $settings['notify_creator'],
  );
  $form['settings']['notify_assignee'] = array(
    '#type' => 'checkbox',
    '#title' => t("Notify the user assigned to the object"),
    '#default_value' => $settings['notify_assignee'],
  );
}

/**
 * Form validation function.
 *
 * Ensure that either:
 * a) At least one workflow role, or
 * b) The creator
 * Has been selected as email recipients.
 * If not, display a form error.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function islandora_workflow_rules_email_someone_validate($form, &$form_state) {
  $settings = $form_state['values']['settings'];
  $has_recipient = !empty($settings['roles']) || $settings['notify_creator'];
  if (!$has_recipient) {
    form_set_error('settings', t('Please select at least one workflow role or creator'));
  }
}

/**
 * Implements hook_rules_event_info().
 *
 * @return array
 *   An array of event information.
 */
function islandora_workflow_rules_event_info() {
  $arguments = array(
    'object' => array(
      'label' => t('Fedora Object'),
      'type' => 'fedora_object',
    ),
  );

  return array(
    'islandora_workflow_rules_reject_object' => array(
      'label' => t('After rejecting a Fedora object'),
      'module' => 'Islandora Rules',
      'arguments' => $arguments,
    ),
    'islandora_workflow_rules_approve_object' => array(
      'label' => t('After approving a Fedora object'),
      'module' => 'Islandora Rules',
      'arguments' => $arguments,
    ),
    'islandora_workflow_rules_submit_object' => array(
      'label' => t('After submitting a Fedora object'),
      'module' => 'Islandora Rules',
      'arguments' => $arguments,
    ),
    'islandora_workflow_rules_assign_object' => array(
      'label' => t('After assigning a Fedora object'),
      'module' => 'Islandora Rules',
      'arguments' => $arguments,
    ),
    'islandora_workflow_rules_delete_object' => array(
      'label' => t('After deleting a Fedora object'),
      'module' => 'Islandora Rules',
      'arguments' => $arguments,
    ),
  );
}

/**
 * Implements hook_token_values().
 */
function islandora_workflow_token_values($type, $object = NULL, $options = array()) {
  $values = array();
  if ('fedora_object' == $type) {
    $values['fedora-object-pid'] = $object->pid;
    $values['fedora-object-title'] = $object->objectProfile->objLabel;
    $values['fedora-object-creator'] = islandora_workflow_get_object_creator($object->pid);
  }
  return $values;
}

/**
 * Implements hook_token_list().
 */
function islandora_workflow_token_list($type = 'all') {
  $tokens = array();
  if ('fedora_object' == $type) {
    $tokens['fedora_object']['fedora-object-pid'] = t("The object PID");
    $tokens['fedora_object']['fedora-object-title'] = t("The object's title");
    $tokens['fedora_object']['fedora-object-creator'] = t("The object's creator");
  }
  return $tokens;
}

/**
 * Implements hook_rules_data_type_info().
 */
function islandora_workflow_rules_data_type_info() {
  $data_types = array();
  $data_types['fedora_object'] = array(
    'label' => t('Fedora object'),
    'class' => 'FedoraObjectRulesDataType',
    'saveable' => FALSE,
    'identifiable' => TRUE,
    'uses_input_form' => FALSE,
    'module' => 'Islandora Workflow',
  );
  return $data_types;
}

class FedoraObjectRulesDataType extends rules_data_type {

  /**
   * Load a Fedora object.
   *
   * @param string $pid
   *   The Fedora PID of the object.
   *
   * @return Fedora_Item
   *   A Fedora object.
   */
  function load($pid) {
    module_load_include('inc', 'fedora_repository', 'api/fedora_item');
    $object = new Fedora_Item($pid);
    return $object;
  }

  /**
   * Get the identifier for the Fedora object.
   *
   * @return mixed
   *   The PID of the Fedora object if it exists; NULL otherwise
   */
  function get_identifier() {
    $fedora_object = $this->get();
    if ($fedora_object) {
      return $fedora_object->pid;
    }
  }

}
