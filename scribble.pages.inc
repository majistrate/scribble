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
 * @return array
 *   The render content array of the snapshot administration.
 */
function scribble_image_list_page($scribble) {
  return entity_view('scribble', array($scribble), 'snapshot_administration');
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

/**
 * Confirm form to remove an image from a scribble.
 *
 * @param object $scribble
 *   The loaded scribble entity.
 * @param int $fid
 *   The file id of the image to remove.
 */
function scribble_remove_image_form($form, &$form_state, $scribble, $fid) {
  // Add file id and scribble id to load in submit handler.
  $form['scribble'] = array(
    '#type' => 'value',
    '#value' => $scribble->sid,
  );
  $form['file_id'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );

  $snapshot_file = file_load($fid - 1);

  // @todo add link to download current scribble images once archive download is implemented.
  // Add info text and snapshot that will be removed.
  $info_text = t('The snapshot below will be removed from all following scribble images of the images created after the deleted will be recreated.');
  $form['info_wrapper'] = array(
    '#type' => 'container',
    'info' => array(
      '#markup' => "<span class='scribble-info-text'>$info_text</span><br />",
    ),
    'snapshot' => array(
      '#markup' => theme('image', array('path' => image_style_url('large', $snapshot_file->uri))),
    ),
  );

  // Load the file to display its name in the question.
  $file = file_load($fid);
  $question =  t('Are you sure to remove image !image from scribble !scribble?', array(
    '!image' => $file->filename,
    '!scribble' => $scribble->label,
  ));
  // Redirect to image list if action get cancelled by the user.
  $path = 'admin/config/media/scribble/' . $scribble->sid . '/image-list';
  return confirm_form($form, $question, $path);
}

/**
 * Submit handler for the remove image confirm form.
 *
 * Removes the image and the corresponding transparent snapshot from the
 * scribble. Afterwards all following snapshots will be recreated and  the
 * file_managed table as well as the entity get updated.
 */
function scribble_remove_image_form_submit($form, &$form_state) {
  // Load the scribble.
  $scribble = entity_load_single('scribble', $form_state['values']['scribble']);

  $fid = $form_state['values']['file_id'];

  // Delete the image and the transparent snapshot files and field items.
  foreach (array('scribble_image', 'scribble_image_snapshots') as $field_name) {
    $items = field_get_items('scribble', $scribble, $field_name);
    // Loop field values to find the file
    foreach ($items as $delta => $item) {
      if ($item['fid'] == $fid) {
        // Save the id of the last clean file for later.
        if ($field_name == 'scribble_image') {
          if ($delta == (count($items) - 1)) {
            // The deleted image is the last image of the scribble, don't set
            // merge base image, there isn't any merging necessary at all.
            $merge_base_resource = FALSE;
          }
          elseif ($delta == 0) {
            // The deleted image is the first image of the scribble, so the base
            // image for recreation is a blank image with background color.
            $merge_base_resource = imagecreatetruecolor($scribble->width, $scribble->height);
            // @todo change the allocated color once background color is configurable.
            $white = imagecolorallocate($merge_base_resource, 255, 255, 255);
            imagefill($merge_base_resource, 0, 0, $white);
            $merge_base_fid = $items[$delta + 1]['uri'];
          }
          else {
            // The deleted image isn't the first nor the last image of the
            // scribble, so the base image is the one before the deleted image.
            $merge_base_resource = imagecreatefrompng($items[$delta - 1]['uri']);
            $merge_base_fid = $items[$delta - 1]['uri'];
          }
        }
        // Load the file to remove.
        $removed_file = file_load($item['fid']);
        // Delete the file from the file_managed table and file system.
        file_delete($removed_file, TRUE);
        // Also remove the record in the field table.
        unset($scribble->{$field_name}[LANGUAGE_NONE][$delta]);
        // The snapshot fid is always the image id - 1 so to remove the snapshot
        // decrease the fid by 1.
        $fid--;
      }
    }
  }

  $scribble->save();

  if ($merge_base_resource !== FALSE) {
    $items = field_get_items('scribble', $scribble, 'scribble_image_snapshots');
    foreach ($items as $item) {
      // Start with merging from the snapshot that follows the snapshot which
      // corresponds the merge base image.
      if ($item['fid'] > ($merge_base_fid - 1)) {
        // Load the snapshot to merge the base with and merge images.
        $merge_snapshot_resource = imagecreatefrompng($item['uri']);
        imagecopy($merge_base_resource, $merge_snapshot_resource, 0, 0, 0, 0, $scribble->width, $scribble->height);

        // imagepng can't deal with stream wrapper uris, create a path.
        $file_name = 'scribble_' . $scribble->sid . '_' . $item['fid'] .  '_' . date('d-m-Y_H-i-s') . '.png';
        $file_path = variable_get('scribble_files_directory', 'sites/default/files/scribble') .  DIRECTORY_SEPARATOR . $scribble->sid . DIRECTORY_SEPARATOR . $file_name;
        imagepng($merge_base_resource, $file_path);

        // Get rid of the resources in order to use the variables again.
        imagedestroy($merge_base_resource);
        imagedestroy($merge_snapshot_resource);

        // Update the image file also in the db.
        $file = file_load($item['fid'] + 1);
        // Delete the old file
        unlink($file->uri);
        // Save the file with the updated image name and uri.
        $file->uri = 'public://scribble' . DIRECTORY_SEPARATOR . $scribble->sid . DIRECTORY_SEPARATOR . $file_name;
        $file->filename = $file_name;
        file_save($file);

        // Use the just created image as merge base for the next image.
        $merge_base_resource = imagecreatefrompng($file->uri);

        // Create image style derivate.
        $derivative_uri = image_style_path('large', $file->uri);
        $style = image_style_load('large');
        image_style_create_derivative($style, $file->uri, $derivative_uri);
      }
    }
  }

  // Redirect to the image list again.
  $form_state['redirect'] = 'admin/config/media/scribble/' . $scribble->sid . '/image-list';
}

/**
 * Page callback for the scribble image slideshow.
 *
 * Will display the images of the a given scribble rendered as slideshow using
 * the formatter of the field_slideshow module if there are any images drawn.
 *
 * @param object $scribble
 *   The scribble entity to display the images of.
 *
 * @return array
 *   Renderable array of the scribble_image field.
 *
 * @todo make this configurable per entity.
 */
function scribble_snapshot_slideshow_page($scribble) {
  // Check if there are any images drawn on the scribble yet.
  $items = field_get_items('scribble', $scribble, 'scribble_image');
  if (!$items) {
    return array(
      '#markup' => t('No images were drawn on this scribble yet.'),
    );
  }

  $display = array(
    'label' => 'hidden',
    'type' => 'slideshow',
    'weight' => '-1',
    'settings' => array(
      'slideshow_image_style' => '',
      'slideshow_link' => '',
      'slideshow_caption' => '',
      'slideshow_caption_link' => '',
      'slideshow_fx' => 'fade',
      'slideshow_speed' => '1000',
      'slideshow_timeout' => '2000',
      'slideshow_order' => '',
      'slideshow_controls' => 0,
      'slideshow_controls_pause' => 1,
      'slideshow_controls_position' => 'after',
      'slideshow_pause' => 1,
      'slideshow_start_on_hover' => 0,
      'slideshow_pager' => 'image',
      'slideshow_pager_position' => 'after',
      'slideshow_pager_image_style' => 'thumbnail',
      'slideshow_carousel_image_style' => '',
      'slideshow_carousel_visible' => '3',
      'slideshow_carousel_scroll' => '1',
      'slideshow_carousel_speed' => '500',
      'slideshow_carousel_skin' => '',
      'slideshow_carousel_follow' => 0,
      'slideshow_carousel_vertical' => 0,
      'slideshow_carousel_circular' => 0,
      'slideshow_colorbox_image_style' => '',
      'slideshow_colorbox_slideshow' => '',
      'slideshow_colorbox_slideshow_speed' => '4000',
      'slideshow_colorbox_transition' => 'elastic',
      'slideshow_colorbox_speed' => '350',
      'slideshow_field_collection_image' => '',
    ),
    'module' => 'field_slideshow',
  );
  return field_view_field('scribble', $scribble, 'scribble_image', $display);
}
