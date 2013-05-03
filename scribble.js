(function($) {

  /**
   * Behavior for scribble module.
   */
  Drupal.behaviors.createBlackboard = {}
  Drupal.behaviors.createBlackboard.attach = function (context, settings) {
    // @todo get the path and filename from the Drupal.settings array
    var options = {
      backgroundImage: 'sites/all/libraries/jqscribble/index.png'
    };
    $('#scribble-canvas').jqScribble(options);

    $('#scribble-save').click(function () {
      $(".scribble-canvas").data("jqScribble").save(function (imageData) {
        if(confirm("This will write a file using the example image_save.php. Is that cool with you?")) {
          $.post(Drupal.settings.scribble.savePath, {imagedata: imageData}, function(response) {
            // @todo print some message and reload the canvas with the new backgroundimage
          });
        }
      });
    });
  }

})(jQuery);
