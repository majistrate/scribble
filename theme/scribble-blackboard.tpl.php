<?php
/**
 * @file
 * Template for the scribble blackboard.
 * @todo document available variables.
 */
print drupal_render_children($form);
?>
<?php print $message_container; ?>
<div class="scribble-toolbar ui-widget-header ui-corner-all">
  <div class="scribble-actions">
    <div class="scribble-save"><?php print t('Save'); ?></div>
    <div class="scribble-clear"><?php print t('Clear'); ?></div>
    <?php if ($allow_image_injection): ?>
      <?php print $image_fetch_txt; ?>
      <?php print $image_file; ?>
      <?php print $image_file_upload_submit; ?>
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
      <?php print $brushes; ?>
    </div>
  </div>
  <div class="scribble-add-img-container"></div>
</div>
<div class="scribble-canvas-wrapper" style="width: <?php print $width; ?>px;height: <?php print $height; ?>px; background-color:#<?php print $background_color; ?>">
  <canvas class="scribble-canvas"></canvas>
</div>
