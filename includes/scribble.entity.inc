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
      $image_items = field_get_items('scribble', $this, 'scribble_image');
      $snapshot_items = field_get_items('scribble', $this, 'scribble_image_snapshots');
      /*
      if (empty($items)) {
        // No images found, display message.
        return array(
          '#markup' => t('No images were added to this scribble yet.'),
        );
      }
      */

      $headers = array(
        t('Image'),
        t('Snapshot'),
        t('Image link'),
        t('Snapshot link'),
        t('Delete'),
      );

      $rows = array();
      // Collect table data of images, snapshots, paths and delete links.
      foreach ($image_items as $delta => $item) {
        // Load the corresponding snapshot file.
        $image_style = variable_get('scribble_image_list_image_style', 'medium');
        $rows[] = array(
          theme('image', array('path' => image_style_url($image_style, $item['uri']))),
          theme('image', array('path' => image_style_url($image_style, $snapshot_items[$delta]['uri']))),
          l($item['filename'], file_create_url($item['uri'])),
          l($snapshot_items[$delta]['filename'], file_create_url($snapshot_items[$delta]['uri'])),
          l(t('Delete'), 'admin/config/media/scribble/' . $this->sid . '/remove-image/' . $item['fid']),
        );
      }

      return array(
        '#theme' =>'table',
        '#header' => $headers,
        '#rows' => $rows,
        '#empty' => t('No strings available.')
      );
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
