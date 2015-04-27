<?php

/**
 * @file
 * Form functions for the Logger module.
 */

/**
 * Implements hook_form_alter().
 */
function logger_form_alter(&$form, &$form_state, $form_id) {
  // In order for hook_form_FORM_ID_alter() implementations to be detected in
  // this file, an implementation of hook_form_alter() is needed, even if empty.
  // @see https://api.drupal.org/comment/8714#comment-8714
}

/**
 * Implements hook_form_FORM_ID_alter() for system_logging_settings().
 */
function logger_form_system_logging_settings_alter(&$form, &$form_state, $form_id) {
  $form['logger_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Logger debugging'),
    '#default_value' => variable_get('logger_debug', FALSE),
    '#description' => t('Log Logger events to Watchdog for debugging. <strong>Warning:</strong> This can add significant load to the database and should not be enabled in production under normal circumstances.'),
  );
}
