<?php

/**
 * @file
 * Definition of ShareMessage entity class.
 */


/*
 * Entity class for the ShareMessage entity.
 */
class ShareMessage extends Entity {

  /**
   * Overrides Entity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $profileid = variable_get('sharemessage_addthis_profile_id', 1);

    $context = array('sharemessage' => $this);
    if ($node = menu_get_object()) {
      $context['node'] = $node;
    }
    drupal_alter('sharemessage_token_context', $this, $context);

    // Add OG Tags to the page.
    $addThis_attributes = FALSE;
    if (strpos(drupal_get_html_head(), 'property="og:') == FALSE && empty($context['_force_attributes'])) {
      $this->addOGTags($context);
    }
    else {
      $addThis_attributes = TRUE;
    }
    unset($context['_force_attributes']);

    // Add addThis buttons.
    $content = array();
    if ($view_mode == 'full') {
      $content['addthis'] = array(
        '#type' => 'container',
        '#attributes' => $addThis_attributes ? $this->buildAdditionalAttributes($context) : $this->buildAttributes(),
        'services' => array(
          '#markup' => $this->build_services_part($context),
        ),
        'additional_services' => array(
          '#markup' => $this->build_additional_services_part(),
        ),
        'addthis_js' => array(
          '#attached' => array(
            'js' => array(
              array(
                'data' => array(
                  'addthis_config' => array(
                    'data_track_addressbar' => TRUE,
                  ),
                ),
                'type' => 'setting',
              ),
              array(
                'data' => '//s7.addthis.com/js/300/addthis_widget.js#pubid=' . $profileid,
                'type' => 'external',
              ),
            ),
          ),
        ),
      );
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }

  /**
   * Function that adds OG tags in the header of the page.
   */
  private function addOGTags($context) {

    // Basic Metadata (og:title, og:type, og:image, og:url).

    // OG: Title.
    $og_title = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:title',
        'content' => $this->getTokenizedField('sharemessage_title', $context),
      ),
    );
    drupal_add_html_head($og_title, 'og_title');

    // OG: Type.
    $og_type = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:type',
        'content' => 'website',
      ),
    );
    drupal_add_html_head($og_type, 'og_type');

    // OG: Image.
    if (isset($this->sharemessage_image[LANGUAGE_NONE][0]['uri'])) {
      $image_url = file_create_url($this->sharemessage_image[LANGUAGE_NONE][0]['uri']);
    }
    else {
      $image_url = $this->getTokenizedField('sharemessage_image_url', $context);
    }
    if ($image_url) {
      $og_image = array(
        '#type' => 'html_tag',
        '#tag' => 'meta',
        '#attributes' => array(
          'property' =>  'og:image',
          'content' => $image_url,
        ),
      );
      drupal_add_html_head($og_image, 'og_image');
    }

    // OG: URL.
    $og_url = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' =>  'og:url',
        'content' =>$this->getTokenizedField('sharemessage_url', $context, url(current_path(), array('absolute' => TRUE))),
      ),
    );
    drupal_add_html_head($og_url, 'og_url');

    // Optional Metadata (og:description).

    // OG: Description.
    $og_description = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'property' => 'og:description',
        'content' => $this->getTokenizedField('sharemessage_long', $context),
      ),
    );
    drupal_add_html_head($og_description, 'og_description');
  }

  /**
   * Function that adds icon style as part of addThis widget.
   */
  private function buildAttributes() {
    $icon_style = !empty($this->settings) ? $this->settings['icon_style'] : variable_get('sharemessage_default_icon_style', 'addthis_16x16_style');
    return array(
      'class' => array('addthis_toolbox', 'addthis_default_style', $icon_style),
    );
  }

  /**
   * Function that adds icon style with addThis:attributes
   * (url, title, description) as part of addThis widget.
   */
  function buildAdditionalAttributes($context) {
    $attributes = $this->buildAttributes();
    $attributes['addthis:url'] = $this->getTokenizedField('sharemessage_url', $context, url(current_path(), array('absolute' => TRUE)));
    $attributes['addthis:title'] = $this->getTokenizedField('sharemessage_title', $context);
    $attributes['addthis:description'] = $this->getTokenizedField('sharemessage_long', $context);
    return $attributes;
  }

  /**
   * Gets a field value and runs it through token_replace().
   *
   * @param $field_name
   * @param $context
   * @param $default
   *
   * @return
   *   If existent, the field value with tokens replace, the default otherwise.
   */
  protected function getTokenizedField($field_name, $context, $default = '') {
    if ($items = field_get_items('sharemessage', $this, $field_name)) {
      return strip_tags(token_replace($items[0]['value'], $context));
    }
    return $default;
  }

  /**
   * Function that adds services as part of addThis widget.
   */
  private function build_services_part($context) {
    $services = !empty($this->settings) ? $this->settings['services'] : variable_get('sharemessage_default_services', array());

    // Configured.
    $services_HTML = '';
    if (!empty($services)) {
      foreach ($services as $key => $service) {
        if ($key == 'twitter' && field_get_items('sharemessage', $this, 'sharemessage_short')) {
          // @todo. This doesn't work, should be printed here.
          drupal_add_js("var addthis_share = { templates: { twitter: '" . $this->getTokenizedField('sharemessage_short', $context) . "', } }", array('type' => 'inline'));
        }
        $services_HTML .= '<a class="addthis_button_' . $key . '"></a>';
      }
    }
    else {
      // Default.
      $services_HTML .= '
        <a class="addthis_button_preferred_1"></a>
        <a class="addthis_button_preferred_2"></a>
        <a class="addthis_button_preferred_3"></a>
        <a class="addthis_button_preferred_4"></a>
        <a class="addthis_button_preferred_5"></a>';
    }

    return $services_HTML;
  }

  /**
   * Function that adds additional services as part of addThis widget.
   */
  private function build_additional_services_part() {
    $additional_services = !empty($this->settings['additional_services']) ? $this->settings['additional_services'] : variable_get('sharemessage_default_additional_services', TRUE);

    $additional = '';
    if ($additional_services) {
      $additional .= '<a class="addthis_button_compact"></a>';
    }
    $counter = isset($this->settings['counter']) ? $this->settings['counter'] : variable_get('sharemessage_default_counter', FALSE);
    if ($counter) {
      $additional .= '<a class="addthis_counter ' . $counter . '"></a>';
    }
    return $additional;
  }
}
