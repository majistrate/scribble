(function($) {
//@todo make the canvas class configurable in the UI and it add as var to JS settings
//@todo make the filename dynamic and save it in the form as hidden element, adapt filename 'scribble.png' here accordingly
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
        if(confirm(Drupal.t('You\'re about to save ur changes. Is that cool with you?'))) {
          $.post(Drupal.settings.scribble.saveURL, {imagedata: imageData}, function(response) {
console.log('RETURNED FROM SAVE AJAX');
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

    var add_image_dragged = false;

    // Register brush handlers
    $('.scribble-brush-btn').click(function () {
      var brush = $(this).attr('rel');
      var brush_name = brush_map[brush];
      $(".scribble-canvas").data("jqScribble").update({brush: brush_name});
      return false;
    });

    // Reset to last saved background image.
    $('.scribble-clear').click(function () {
      var options = {
        backgroundImage: Drupal.settings.scribble.bgImagePath + '/scribble.png'
      };
      $('.scribble-canvas').data('jqScribble').update(options);
      return false;
    });

    // Handle image add action.
    $('.scribble-add').click(function () {
      console.log(event);
      var img_src = prompt(Drupal.t('Enter the URL of the image.'));
      if(img_src !== '') {
        var $info = $('<div>' + Drupal.t('Drag the image on the blackboard in order to add it.') + '</div>');
        $('.scribble-add-img-container').html($info);
        var $img = $('<img />');
        $img.attr({
          'src': img_src,
          'class': 'scribble-add-image'
        });
        $('.scribble-add-img-container').append($img);
        var options = {
          stop: addImgStopHandler,
          revert: true
        };
        $img.draggable(options);
      }
      return false;
    });

    function addImgStopHandler(event, ui) {
      // @todo calculate mouse offset from dragged img into x and y
      // @todo check if the img was dropped within the canvas dimensions
      var x = event.pageX - $('.scribble-canvas').offset().left;
      var y = event.pageY - $('.scribble-canvas').offset().top;
      var data = {
        img_url: $('.scribble-add-image').attr('src'),
        img_width: $('.scribble-add-image').width(),
        img_height: $('.scribble-add-image').height(),
        dst_x: x,
        dst_y: y
      };
      $.post(Drupal.settings.scribble.addURL, data, function(response) {
        var options = {
          backgroundImage: Drupal.settings.scribble.bgImagePath + '/scribble.png'
        };
        $('.scribble-canvas').data('jqScribble').update(options);
      });
    }
  }

})(jQuery);
