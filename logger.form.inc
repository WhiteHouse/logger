<?php
/**
 * @file
 * Form functions for the Logger module.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function logger_form_system_logging_settings_alter(&$form, &$form_state, $form_id) {
  $form['logger_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Logger debugging'),
    '#options' => array(
      'logger_debug',
    ),
    '#description' => t('Enabling this option will log Logger events to Watchdog.'),
  );
}