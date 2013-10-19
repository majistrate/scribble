<?php
/**
 * Template for the scribble blackboard.
 * @todo make brushes configurable and loop them.
 * @todo apply width and height to .scribble-canvas-wrapper element
 */

?>
<div class="scribble-blackboard-wrapper">
  <div class="scribble-toolbar ui-widget-header ui-corner-all">
    <div class="scribble-actions">
      <div class="scribble-save"><?php print t('Save'); ?></div>
      <div class="scribble-clear"><?php print t('Clear'); ?></div>
      <?php if (variable_get('scribble_allow_image_injection', 0)): ?>
        <input type="text" value="http://www.istos.it/sites/default/files/istoslogo.png" class="ui-corner-all" id="img-src-txt" name="scribble-img-src" placeholder="<?php print t('Enter the URL of the image to add'); ?>" />
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
          $first = TRUE;
          foreach ($brushes as $brush_id => $label) {
            $brush_btn = '<input type="radio" id="' . $brush_id . '" name="brushes-btns" ';
            if ($first) {
              $brush_btn .= 'checked="checked"';
              $first = FALSE;
            }
            $brush_btn .= ' /><label for="' . $brush_id . '">'  . $label . '</label>';
            print $brush_btn;
          }
        ?>
      </div>
    </div>
    <div class="scribble-add-img-container"></div>
  </div>
  <div class="scribble-canvas-wrapper" style="width: <?php print $width; ?>px;height: <?php print $height; ?>px;">
    <canvas class="scribble-canvas"></canvas>
  </div>
</div>