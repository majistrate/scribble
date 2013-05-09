(function($) {
//@todo make the canvas class configurable in the UI and it add as var to JS settings
  /**
   * Behavior for scribble module.
   */
  Drupal.behaviors.createBlackboard = {};
  Drupal.behaviors.createBlackboard.attach = function (context, settings) {

    // Load the default image.
    var options = {
      backgroundImage: Drupal.settings.scribble.bgImagePath + '/scribble.png'
    };
    $('.scribble-canvas').jqScribble(options);

    // Register the handler for the save action.
    $('.scribble-save').click(function () {
      $(".scribble-canvas").data("jqScribble").save(function (imageData) {
        if(confirm("You're about to save ur changes. Is that cool with you?")) {
          $.post(Drupal.settings.scribble.saveURL, {imagedata: imageData}, function(response) {
            console.log('RETURNED FROM AJAX');
            var options = {
              backgroundImage: Drupal.settings.scribble.bgImagePath + '/scribble.png'
            };
            $('.scribble-canvas').data('jqScribble').update(options);
          });
        }
      });
      return false;
    });

    var brush_map = {
      'line': LineBrush,
      'cross': CrossBrush,
      'basic': BasicBrush
    };

    // Register brush handlers
    $('.scribble-brush-btn').each(function () {
      $(this).click(function () {
        var brush = $(this).attr('rel');
        var brush_name = brush_map[brush];
        console.log(brush_name);
        $(".scribble-canvas").data("jqScribble").update({brush: brush_name});
        return false;
      });
    });
  }

})(jQuery);
