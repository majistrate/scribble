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
  $output .= '<div class="scribble-brushes">' . t('Brush') . '</div>';
  $output .= '<div class="scribble-add-image">' . t('Add image') . '</div>';
  $output .= '<div class="scribble-brush-size"></div>';
  $output .= '<div class="scribble-brush-size-display ui-corner-all"></div>';
  $output .= '</div>';

  $output .= '<div class="scribble-toolbar-modal-content">';
  // Render add image fields only if this was explicitly enabled globbally.
  if (variable_get('scribble_allow_image_injection', FALSE)) {
    $output .= '<div class="scribble-image-injection-wrapper">';
    $output .= $variables['image_fetch_txt'];
    $output .= $variables['image_file'];
    $output .= $variables['image_file_upload_submit'];
    // The container used to drag images from into the canvas.
    $output .= '<div class="scribble-add-img-modal"></div>';
    $output .= '</div>';
  }

  // Brush settings and selection.
  $output .= '<div class="scribble-brush-settings">';
  $output .= '<div class="scribble-color-options-wrapper">';
  $output .= '<div class="scribble-color-btn">' . t('Color') . '</div>';
  $output .= '<div class="scribble-color-display ui-corner-all"></div>';
  $output .= '</div>';
  $output .= '<div class="scribble-color-picker"></div>';
  $output .= '<div class="scribble-brush-buttons-wrapper">';
  // Add brush buttons.
  $output .= theme('scribble_brush_buttons', array(
    'brushes' => scribble_get_enabled_brushes($scribble))
  );
  $output .= '</div>';
  $output .= '</div>';
  $output .= '</div>';

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

/**
 * Theme function for the brush buttons..
 */
function theme_scribble_brush_buttons($variables) {
  $output = '<div class="scribble-brush-buttons-wrapper">';
  foreach ($variables['brushes'] as $key => $label) {
    $output .= '<div class="scribble-brush-button"';
    $output .= 'data-brush="' . $key . '" title="' . $label . '">';
    $output .= theme('image', array(
      'path' => drupal_get_path('module', 'scribble') . '/theme/img/' . strtolower($label) . '.png',
      'alt' => $label,
    ));
    $output .= '</div>';
  }
  return $output . '</div>';
}
