<?php

/**
 * @file
 * Contains the page and form callbacks for the scribble module.
 */

/**
 * Form callback that contains the blackboard.
 * // @todo remove this.
 */
function scribble_blackboard_form($form, $form_state) {
  $form['#attached']['library'][] = array('system', 'ui.draggable');
  $form['#attached']['library'][] = array('system', 'farbtastic');
  $form['#attached']['library'][] = array('system', 'ui.slider');
  $form['#attached']['library'][] = array('system', 'ui.button');
  $form['#attached']['library'][] = array('system', 'ui.dialog');
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
 * System configuration form for the scribble module.
 */
function scribble_configuration_form($form, &$form_state) {
  $form['scribble_allow_image_injection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow adding images'),
    '#description' => t("Do only allow this if your scribbles aren't public accessible. This might cause legal issues otherwise if images with copyrights get added."),
    '#default_value' => variable_get('scribble_allow_image_injection', 0),
  );

  return system_settings_form($form);
}

/**
 * Form callback that contains the blackboard.
 */
function scribble_page($scribble) {
dpm($scribble);
  dpm(entity_get_info('scribble'));
  return entity_view('scribble', array($scribble));
}

function scribble_image_list_page($scribble) {
  dpm($scribble);
  return array();
}

/**
 * AJAX callback for the "Save" button.
 *
 * Creates an image from the data passed by via post request.
 * // @todo make the folder /scribble configurable on the settings form.
 */
function scribble_save_ajax() {
  $scribble_id = $_POST['scribble_id'];
  if (!empty($_POST['imagedata']) && !empty($scribble_id)) {
    $data = $_POST['imagedata'];

    // Create directory on runtime if it doesn't exists.
    $path = variable_get('file_public_path', 'sites/default/files') . '/scribble';
    if (!file_exists($path)) {
      drupal_mkdir($path, 0777);
      variable_set('scribble_files_directory', $path);
    }

    // Create a separate folder for the entity if it doesn't exist.
    $path = $path . '/' . $scribble_id . '/';
    if (!file_exists($path)) {
      drupal_mkdir($path, 0777);
    }
    $file_name = 'scribble_' . $scribble_id . '_' . date('d-m-Y_H-i-s') . '.png';

    // Need to remove the stuff at the beginning of the image data string.
    $data = substr($data, strpos($data, ",")+1);

    // Decode the data, create an image resource and save image.
    $data = base64_decode($data);
    $img_resource = imagecreatefromstring($data);
    if (!empty($img_resource)) {
      imagepng($img_resource, $path . $file_name);
    }

    // Prepare filedata.
    $file_temp = file_get_contents($path . $file_name);
    $file_temp = file_save_data($file_temp, 'public://scribble/' . $scribble_id . '/' . $file_name, FILE_EXISTS_REPLACE);

    // Load the scribble and add image to image field of the scribble entity.
    $scribble = entity_load_single('scribble', $scribble_id);
    $scribble->scribble_image[LANGUAGE_NONE][] = array(
      'fid' => $file_temp->fid,
      'filename' => $file_temp->filename,
      'filemime' => $file_temp->filemime,
      'uid' => 1,
      'uri' => $file_temp->uri,
      'status' => 1,
    );
    $scribble->save();

    // Destroy resource and print new filename as JSON.
    imagedestroy($img_resource);
    drupal_json_output(array('file_name' => $file_name));
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
    $img_path = variable_get('scribble_files_directory', 'sites/default/files/scribble') . '/';
    $file_name = scribble_get_newest();
    $img_resource = !empty($file_name) ? imagecreatefrompng($img_path . $file_name) : imagecreatetruecolor($_POST['canvas_width'], $_POST['canvas_height']);
    if (!empty($img_resource) && !empty($add_img_resource)) {
      imagecopy($img_resource, $add_img_resource, $_POST['dst_x'], $_POST['dst_y'], 0, 0, $_POST['img_width'], $_POST['img_height']);
      $file_name = 'scribble_' . date('d-m-Y_H-i-s') . '.png';
      imagepng($img_resource, $img_path . $file_name);
    }
    imagedestroy($img_resource);
    imagedestroy($add_img_resource);
    drupal_json_output(array('file_name' => $file_name));
    drupal_exit();
  }
}
