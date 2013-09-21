<?php

/**
 * @file
 * Definition of ShareMessage entity class.
 */


/*
 * Entity class for the ShareMessage entity.
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
          'height' => $this->height,
          'width' => $this->width,
          'class' => array('scribble-blackboard-wrapper'),
        ),
        'blackboard' => array(
          '#markup' => theme('scribble_blackboard', array()),
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
          ),
        ),
      );
    }
    /* @todo
    elseif ($view_mode == 'images_only') {

    }
    */

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
