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

  $form['board'] = array(
    '#markup' => theme('scribble_blackboard', array()),
  );

  // @todo add element for the entity id
/*
  $form['save'] = array(
    '#type' => 'button',
    '#value' => t('Save'),
    '#attributes' => array(
      'class' => array('scribble-save'),
    ),
  );
*/
  return $form;
}

/**
 * AJAX callback for the "Save" button.
 *
 * Creates an image from the data passed by via post request.
 */
function scribble_save_ajax() {
  if (!empty($_POST['imagedata'])) {
    $data = $_POST['imagedata'];
    // @todo create a folder for every scribble entity and save it there
    // Create directory on runtime if it doesn't exists.
    $path = variable_get('file_public_path', 'sites/default/files') . '/scribble';
    if (!file_exists($path)) {
      drupal_mkdir($path, 0777);
      variable_set('scribble_files_directory', $path);
    }
    $file_name = variable_get('scribble_files_directory', 'sites/default/files/scribble') . '/scribble.png';
    // Need to remove the stuff at the beginning of the string.
    $data = substr($data, strpos($data, ",")+1);
    // Create an image resource and save and image.
    $data = base64_decode($data);
    $img_resource = imagecreatefromstring($data);
    if (!empty($img_resource)) {
      imagepng($img_resource, $file_name);
    }
    imagedestroy($img_resource);
    drupal_exit();
  }
}
