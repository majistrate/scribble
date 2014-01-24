<?php
/**
 * @file
 * Template file for the scribble brush options on the blackboard toolbar.
 *
 * Variables:
 *  - $brushes: The brush options as configured on the scribble entity.
 */
$first = TRUE;
foreach ($variables['brushes'] as $brush_id => $label):
  if ($first) {
    $checked = 'checked="checked"';
    $first = FALSE;
  }
  else {
    $checked = '';
  }
?>
  <input type="radio" id="<?php print $brush_id; ?>" name="brushes-btns" <?php print $checked; ?>/>
  <label for="<?php print $brush_id; ?>"><?php print $label; ?></label>
<?php endforeach; ?>
