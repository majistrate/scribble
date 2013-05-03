(function($) {

  /**
   * Behavior for scribble module.
   */
  Drupal.behaviors.createBlackboard = {}
  Drupal.behaviors.createBlackboard.attach = function (context, settings) {
    $('.scribble-canvas').jqScribble();
  }

})(jQuery);
