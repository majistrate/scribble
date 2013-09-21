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
                'data' => array('scribble_id' => $this->sid),
                'type' => 'setting',
              ),
            ),
          ),
        ),
      );
    }
    /* @todo implement images list with delete links.
    elseif ($view_mode == 'images_only') {

    }
    */

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
