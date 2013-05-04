<?php
/**
 *
 */
?>
<div class="scribble-blackboard-wrapper">
  <canvas class="scribble-canvas"></canvas>
  <div class="scribble-actions">
    <a href="#" class="scribble-save"><?php print t('Save'); ?></a>
    <a href="#" class="scribble-add">Add Image</a>
    <a href="#" class="scribble-clear">Clear</a>
  </div>
  <div class="scribble-brushes">
    <strong>BRUSHES:</strong>
    <a href="#" onclick='$("#test").data("jqScribble").update({brush: BasicBrush});'>Basic</a>
    <a href="#" onclick='$("#test").data("jqScribble").update({brush: LineBrush});'>Line</a>
    <a href="#" onclick='$("#test").data("jqScribble").update({brush: CrossBrush});'>Cross</a>
  </div>
</div>