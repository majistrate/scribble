<?php
/**
 * Template for the scribble blackboard.
 * // @todo make brushed configurable.
 */

?>
<div class="scribble-blackboard-wrapper">
  <canvas class="scribble-canvas"></canvas>
  <div class="scribble-actions">
    <a href="#" class="scribble-save"><?php print t('Save'); ?></a>
    <a href="#" class="scribble-add"><?php print t('Add Image'); ?></a>
    <a href="#" class="scribble-clear"><?php print t('Clear'); ?></a>
  </div>
  <div class="scribble-brushes">
    <strong><?php print t('Brushes'); ?></strong>
    <a href="#" class="scribble-brush-btn" rel="basic"><?php print t('Basic'); ?></a>
    <a href="#" class="scribble-brush-btn" rel="line"><?php print t('Line'); ?></a>
    <a href="#" class="scribble-brush-btn" rel="cross"><?php print t('Cross'); ?></a>
  </div>
</div>
