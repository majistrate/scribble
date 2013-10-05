<?php

/**
 * @file
 * Definition of Scribble entity class.
 */

/*
 * Entity class for the Scribble entity.
 */
class Scribble extends Entity {

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = array();
    if ($view_mode == 'full') {
      $content['blackboard'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('scribble-blackboard-wrapper'),
        ),
        'blackboard' => array(
          '#markup' => theme('scribble_blackboard', array(
            'width' => $this->width,
            'height' => $this->height,
            'brushes' => $this->prepareBrushes(),
          )),
        ),
        'scribble_libraries' => array(
          '#attached' => array(
            'library' => array(
              array('system', 'ui.draggable'),
              array('system', 'farbtastic'),
              array('system', 'ui.slider'),
              array('system', 'ui.button'),
              array('system', 'ui.dialog'),
              array('scribble', 'jqscribble'),
            ),
            'js' => array(
              array(
                'data' => array(
                  'scribble_info' => array(
                    'scribbleId' => $this->sid,
                    'newestScribble' => scribble_get_newest($this->sid),
                  ),
                ),
                'type' => 'setting',
              ),
            ),
          ),
        ),
      );
    }
    elseif ($view_mode == 'snapshot_administration') {
      $items = field_get_items('scribble', $this, 'scribble_image');
      if (empty($items)) {
        // No images found, display message.
        return array(
          '#markup' => t('No images were added to this scribble yet.'),
        );
      }

      // Print a list of images and delete links.
      foreach ($items as $delta => $item) {
        $content['item_' . $delta . '_wrapper'] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array('scribble-snapshot-admin-wrapper'),
          ),
          'image_' . $delta . '_container' => array(
            '#markup' => theme('image', array(
              'path' => image_style_url('large', $item['uri']),
            )),
          ),
          'delete_link' => array(
            '#markup' => l(t('Delete'), 'scribble/' . $this->sid . '/delete-image/' . $delta),
          ),
        );
      }
      return $content;
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Returns the enabled brushes.
   *
   * @return array
   *   Brush labels indexed by brush JS class name.
   */
  protected function prepareBrushes() {
    $items = field_get_items('scribble', $this, 'scribble_brushes');
    $brushes = scribble_get_brushes();
    $return = array();
    foreach ($items as $item) {
      $return[$item['value']] = $brushes[$item['value']];
    }
    return $return;
  }
}
