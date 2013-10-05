<?php
/**
 * @file
 * Contains the page and form callbacks for the scribble module.
 */

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

/**
 * Page callback for the snapshot administration.
 *
 * @param object $scribble
 *   The loaded scribble entity.
 *
 * @return bool
 *
 */
function scribble_image_list_page($scribble) {
  return $scribble->buildContent('snapshot_administration');
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
    $path = variable_get('file_public_path', 'sites/default/files') . DIRECTORY_SEPARATOR . 'scribble';
    if (!file_exists($path)) {
      drupal_mkdir($path, 0777);
      variable_set('scribble_files_directory', $path);
    }

    // Create a separate folder for the entity if it doesn't exist.
    $path = $path . DIRECTORY_SEPARATOR . $scribble_id . DIRECTORY_SEPARATOR;
    if (!file_exists($path)) {
      drupal_mkdir($path, 0777);
    }
    $snapshot_path = $path . DIRECTORY_SEPARATOR . 'snapshots' . DIRECTORY_SEPARATOR;
    if (!file_exists($snapshot_path)) {
      drupal_mkdir($snapshot_path, 0777);
    }
    $file_name = 'scribble_' . $scribble_id . '_' . date('d-m-Y_H-i-s') . '.png';
    $snapshot_file_name = 'scribble_snapshot_' . $scribble_id . '_' . date('d-m-Y_H-i-s') . '.png';

    // Load the scribble entity.
    $scribble = entity_load_single('scribble', $scribble_id);

    // Need to remove the stuff at the beginning of the image data string.
    $data = substr($data, strpos($data, ",") + 1);

    // Decode the data, create an image resource and save image.
    $data = base64_decode($data);
    $snapshot_resource = imagecreatefromstring($data);
    if (!empty($snapshot_resource)) {
      // Make the background transparent.
      imagesavealpha($snapshot_resource, TRUE);
      // Write the snapshot file.
      imagepng($snapshot_resource, $snapshot_path . $snapshot_file_name);
      imagedestroy($snapshot_resource);

      // Merge the created snapshot with the latest image. This is necessary
      // because there might be a newer image saved than the loaded background
      // on the scribble. That way, overwriting the latest image is prevented.
      $snapshot_resource = imagecreatefrompng($snapshot_path . $snapshot_file_name);
      if ($latest_file_name = scribble_get_newest($scribble_id)) {
        $background_resource = imagecreatefrompng($path . $latest_file_name);
      }
      else {
        $background_resource = imagecreatetruecolor($scribble->width, $scribble->height);
        // @todo change the allocated color once background color is configurable.
        $white = imagecolorallocate($background_resource, 255, 255, 255);
        imagefill($background_resource, 0, 0, $white);
      }
      imagecopy($background_resource, $snapshot_resource, 0, 0, 0, 0, $scribble->width, $scribble->height);

      // Write the new scribble image file and destroy resources.
      imagepng($background_resource, $path . $file_name, 0);
      imagedestroy($background_resource);
      imagedestroy($snapshot_resource);

      // Add the images and snapshot to the fields of the scribble entity.
      scribble_add_image_to_scribble_field('public://scribble/' . $scribble_id . '/snapshots/' . $snapshot_file_name, $scribble, 'scribble_image_snapshots');
      scribble_add_image_to_scribble_field('public://scribble/' . $scribble_id . DIRECTORY_SEPARATOR . $file_name, $scribble, 'scribble_image');
    }

    // Print new filename as JSON to update the div behind the canvas.
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
