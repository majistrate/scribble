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
      <input type="text" value="http://www.istos.it/sites/default/files/istoslogo.png" class="ui-corner-all" id="img-src-txt" name="scribble-img-src" placeholder="<?php print t('Enter the URL of the image to add'); ?>" />
      <div class="scribble-add"><?php print t('Add Image'); ?></div>
    </div>
    <hr class="scribble-separator" />
    <div class="scribble-brush-settings">
      <div class="scribble-color-btn"><?php print t('Color'); ?></div>
      <div class="scribble-color-display ui-corner-all"></div>
      <div class="scribble-color-picker"></div>
      <div class="scribble-brush-size"></div>
      <div class="scribble-brush-size-display ui-corner-all"></div>
      <div class="scribble-brushes">
        <input type="radio" id="basic" name="brushes-btns" checked="checked" /><label for="basic"><?php print t('Basic'); ?></label>
        <input type="radio" id="line" name="brushes-btns" /><label for="line"><?php print t('Line'); ?></label>
        <input type="radio" id="cross" name="brushes-btns" /><label for="cross"><?php print t('Cross'); ?></label>
      </div>
    </div>
    <div class="scribble-add-img-container"></div>
  </div>
  <div class="scribble-canvas-wrapper">
    <canvas class="scribble-canvas" />
  </div>
</div>
