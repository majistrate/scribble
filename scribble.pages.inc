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

    // Load the scribble entity.
    $scribble = entity_load_single('scribble', $scribble_id);

    // Need to remove the stuff at the beginning of the image data string.
    $data = substr($data, strpos($data, ",")+1);

    // @todo load the latest image and merge with posted image data, then save that image in the field
    // @todo merge that transparent image with a white background and store it in another field.
    // @todo make the background color configurable.
    // Decode the data, create an image resource and save image.
    $data = base64_decode($data);
    $img_resource = imagecreatefromstring($data);
    if (!empty($img_resource)) {
      // Make the background transparent, the transparent canvas will send other
      // transparent parts as #000 (black).
      $black = imagecolorallocate($img_resource, 0, 0, 0);
      imagecolortransparent($img_resource, $black);

      // Merge the created image with the latest image. This is necessary
      // because there might be a newer image saved than the loaded background
      // on the scribble. That way, overwriting the latest image is prevented.
      if ($latest_file_name = scribble_get_newest($scribble_id)) {
        $latest_img_resource = imagecreatefrompng($path . $latest_file_name);
        imagecopy($latest_img_resource, $img_resource, 0, 0, 0, 0, $scribble->width, $scribble->height);
      }
      imagepng($latest_img_resource, $path . 'test.png');
      // Write the image file.
      imagepng($img_resource, $path . $file_name);
    }

    // Add the image to the image field of the scribble entity.
    scribble_add_image_to_scribble($path, $file_name, $scribble);

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
