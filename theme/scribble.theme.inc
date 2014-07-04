<?php
/**
 * @file
 * Theme function for the scribble module
 */

/**
 * Theming for the blackboard toolbar form.
 */
function theme_scribble_blackboard($variables) {
  $form = $variables['form'];
  $scribble = $form['#scribble'];

  // Pull the configured background color.
  $items = field_get_items('scribble', $variables['form']['#scribble'], 'scribble_background_color');
  $background_color = $items[0]['jquery_colorpicker'];

  // Action buttons.
  $output = $variables['message_container'];
  $output .= '<div class="scribble-toolbar ui-widget-header ui-corner-all">';
  $output .= '<div class="scribble-actions">';
  $output .= '<div class="scribble-save">' . t('Save') . '</div>';
  $output .= '<div class="scribble-clear">' . t('Clear') . '</div>';
  $output .= '</div>';

  // Render add image fields only if this was explicitly enabled globbally.
  if (variable_get('scribble_allow_image_injection', FALSE)) {
    $output .= '<div class="scribble-actions">';
    $output .= $variables['image_fetch_txt'];
    $output .= $variables['image_file'];
    $output .= $variables['image_file_upload_submit'];
    $output .= '</div>';
  }

  // Brush settings and selection.
  $output .= '<hr class="scribble-separator" />';
  $output .= '<div class="scribble-brush-settings">';
  $output .= '<div class="scribble-color-btn">' . t('Color') . '</div>';
  $output .= '<div class="scribble-color-display ui-corner-all"></div>';
  $output .= '<div class="scribble-color-picker"></div>';
  $output .= '<div class="scribble-brush-size"></div>';
  $output .= '<div class="scribble-brush-size-display ui-corner-all"></div>';
  $output .= '<div class="scribble-brushes"></div>';
  $output .= '</div>';
  // Add brush radios.
  $output .= $variables['brushes'];

  // The container used to drag images from into the canvas.
  $output .= '<div class="scribble-add-img-container"></div>';
  $output .= '</div>';
  // The actual canvas with wrapper for the configure width, height and bg col.
  $output .= '<div class="scribble-canvas-wrapper" style="width:' . $scribble->width . 'px;height: ' . $scribble->height . 'px; background-color:#' . $background_color . '">';
  $output .= '<canvas class="scribble-canvas"></canvas>';
  $output .= '</div>';

  // Render remaining form elements.
  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Theme function for the slideshow output.
 */
function theme_scribble_slideshow($variables) {
  return '<div class="scribble-slideshow-wrapper">' . render($variables['slideshow']) . '</div>';
}
