<?php

/**
 * @file
 * Contains the page and form callbacks for the scribble module.
 */

/**
 * Form callback that contains the blackboard.
 */
function scribble_blackboard_form($form, $form_state) {
  $form['#attached']['library'][] = array('scribble', 'jqscribble');

  $form['canvas'] = array(
    '#markup' => '<canvas id="scribble-canvas"></canvas>',
  );

  $form['save'] = array(
    '#type' => 'button',
    '#value' => t('Save'),
    '#id' => 'scribble-save',
  );
  return $form;
}