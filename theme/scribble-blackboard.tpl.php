<?php
/**
 * Template for the scribble blackboard.
 * @todo make brushes configurable and loop them.
 */

?>
<div class="scribble-blackboard-wrapper">
  <canvas class="scribble-canvas"></canvas>
  <div class="scribble-toolbar ui-widget-header ui-corner-all">
    <div class="scribble-actions">
      <a href="#" class="scribble-save"><?php print t('Save'); ?></a>
      <a href="#" class="scribble-add"><?php print t('Add Image'); ?></a>
      <a href="#" class="scribble-clear"><?php print t('Clear'); ?></a>
      <div class="scribble-color-btn"><?php print t('Color'); ?></div>
      <div class="scribble-color-display ui-corner-all"></div>
      <div class="scribble-color-picker"></div>
      <div class="scribble-brush-size"></div>
    </div>
    <div class="scribble-brushes">
      <input type="radio" id="basic" name="brushes-btns" checked="checked" /><label for="basic"><?php print t('Basic'); ?></label>
      <input type="radio" id="line" name="brushes-btns" /><label for="line"><?php print t('Line'); ?></label>
      <input type="radio" id="cross" name="brushes-btns" /><label for="cross"><?php print t('Cross'); ?></label>
    </div>
    <div class="scribble-add-img-container"></div>
  </div>
</div>
