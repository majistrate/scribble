<?php
/**
 * @file
 * Theme function for the scribble module
 */

/**
 * Theming for the blackboard toolbar form.
 */
function theme_scribble_blackboard($variables) {
  $form = $variables['render element'];
  $test = 1;
/*
<div class="scribble-blackboard-wrapper">
  <div class="scribble-toolbar ui-widget-header ui-corner-all">
    <div class="scribble-actions">
      <div class="scribble-save"><?php print t('Save'); ?></div>
<div class="scribble-clear"><?php print t('Clear'); ?></div>
<?php if (variable_get('scribble_allow_image_injection', 0)): ?>
  <input type="text" value="" class="ui-corner-all" id="img-src-txt" name="scribble-img-src" placeholder="<?php print t('Enter the URL of the image to add'); ?>" />
  <div class="scribble-add"><?php print t('Add Image'); ?></div>
<?php endif; ?>
</div>
<hr class="scribble-separator" />
<div class="scribble-brush-settings">
  <div class="scribble-color-btn"><?php print t('Color'); ?></div>
  <div class="scribble-color-display ui-corner-all"></div>
  <div class="scribble-color-picker"></div>
  <div class="scribble-brush-size"></div>
  <div class="scribble-brush-size-display ui-corner-all"></div>
  <div class="scribble-brushes">
    <?php
    print theme('scribble_brush_options', array('brushes' => $brushes));
    ?>
  </div>
</div>
<div class="scribble-add-img-container"></div>
</div>
<div class="scribble-canvas-wrapper" style="width: <?php print $width; ?>px;height: <?php print $height; ?>px; background-color:#<?php print $background_color; ?>">
  <canvas class="scribble-canvas"></canvas>
</div>
</div>*/
}


/**
 * Theme function for the scribble brush options on the blackboard toolbar.
 */
function theme_scribble_brush_options($variables) {
  $first = TRUE;
  $output = '';
  foreach ($variables['brushes'] as $brush_id => $label) {
    if ($first) {
      $checked = ' checked="checked"';
      $first = FALSE;
    }
    else {
      $checked = '';
    }
    $output .= '<input type="radio" id="' . $brush_id . '" name="brushes-btns"' . $checked . '/>';
    $output .= '<label for="' . $brush_id . '">' . $label . '</label>';
  }
  return $output;
}

/**
 * Theme function for the slideshow output.
 */
function theme_scribble_slideshow($variables) {
  return '<div class="scribble-slideshow-wrapper">' . render($variables['slideshow']) . '</div>';
}
