<?php

/**
 * @file
 * Contains the page and form callbacks for the scribble module.
 */

/**
 * Form callback that contains the blackboard.
 */
function scribble_blackboard_form($form, $form_state) {
  $form['#attached']['library'][] = array('system', 'ui.draggable');
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

/**
 * AJAX callback for the "Save" button.
 *
 * Creates an image from the data passed by via post request.
 */
function scribble_add_ajax() {
  dd($_POST, 'POST');
  // @todo just return an image, don't save right here.
  // @todo check more post values
  if (!empty($_POST['img_url'])) {
    // Get the image resource of the image that gets added.
    $url_parts = explode('/', $_POST['img_url']);
    $file_name = array_pop($url_parts);
    $extension = explode('.', $file_name);
    $extension = str_replace('jpg', 'jpeg', $extension[1]);
    $function = 'imagecreatefrom' . $extension;
    if (!function_exists($function)) {
      return;
    }
    $add_img_resource = $function($_POST['img_url']);
    // @todo create a folder for every scribble entity and save it there
    // Create directory on runtime if it doesn't exists.
    $path = variable_get('file_public_path', 'sites/default/files') . '/scribble';
    if (!file_exists($path)) {
      drupal_mkdir($path, 0777);
      variable_set('scribble_files_directory', $path);
    }
    $file_name = variable_get('scribble_files_directory', 'sites/default/files/scribble') . '/scribble.png';
    $img_resource = imagecreatefrompng($file_name);
    if (!empty($img_resource) && !empty($add_img_resource)) {
      imagecopy($img_resource, $add_img_resource, $_POST['dst_x'], $_POST['dst_y'], 0, 0, $_POST['img_width'], $_POST['img_height']);
      $success = imagepng($img_resource, $file_name);
      dd($success, 'TEST');
    }
    imagedestroy($img_resource);
    imagedestroy($add_img_resource);
    drupal_exit();
  }
}
