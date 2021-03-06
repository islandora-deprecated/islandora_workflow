<?php
/**
 * @file
 *   A set of sorting functions for the workflow table.
 */

/**
 * String comparison function for workflow label. Sorts Ascending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_label_asc($a, $b) {
  return strnatcasecmp($a['label'], $b['label']);
}

/**
 * String comparison function for workflow label. Sorts Decending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_label_desc($a, $b) {
  return strnatcasecmp($b['label'], $a['label']);
}

/**
 * String comparison function for workflow collection label. Sorts Ascending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_collection_label_asc($a, $b) {
  return strnatcasecmp($a['collection_label'], $b['collection_label']);
}

/**
 * String comparion function for workflow collection label. Sorts decending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_collection_label_desc($a, $b) {
  return strnatcasecmp($b['collection_label'], $a['collection_label']);
}

/**
 * String comparison function for workflow state. Sorts Ascending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_state_asc($a, $b) {
  return strnatcasecmp($a['state'], $b['state']);
}

/**
 * String comparison function for workflow state. Sorts decending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_state_desc($a, $b) {
  return strnatcasecmp($b['state'], $a['state']);
}

/**
 * String comparison function for workflow created_date. Sorts Ascending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_created_date_asc($a, $b) {
  return strtotime($a['created_date']) - strtotime($b['created_date']);
}

/**
 * String comparison function for workflow created_date. Sorts decending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return int
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_created_date_desc($a, $b) {
  return strtotime($b['created_date']) - strtotime($a['created_date']);
}

/**
 * String comparison function for workflow modified date. Sorts Ascending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_islandora_workflow_modified_asc($a, $b) {
  return strtotime($a['islandora_workflow_modified']) - strtotime($b['islandora_workflow_modified']);
}

/**
 * String comparison function for workflow modified date. Sorts decending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_islandora_workflow_modified_desc($a, $b) {
  return strtotime($b['islandora_workflow_modified']) - strtotime($a['islandora_workflow_modified']);
}

/**
 * String comparison function for workflow assignee. Sorts Ascending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return int
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_assignee_asc($a, $b) {
  return strnatcasecmp($a['assignee'], $b['assignee']);
}

/**
 * String comparison function for workflow assignee. Sorts descending.
 *
 * @param string $a
 *   String A
 * @param string $b
 *   String B
 *
 * @return integer
 *   < 0 if str1 is less than str2 > 0 if str1 is greater than str2
 */
function islandora_workflow_sort_assignee_desc($a, $b) {
  return strnatcasecmp($b['assignee'], $a['assignee']);
}
