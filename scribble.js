(function($) {
//@todo make the canvas class configurable in the UI and it add as var to JS settings
//@todo make the filename dynamic and save it in the form as hidden element, adapt filename 'scribble.png' here accordingly
  /**
   * Behavior for scribble module.
   */
  Drupal.behaviors.createBlackboard = {};
  Drupal.behaviors.createBlackboard.attach = function (context, settings) {

    var drag_img_offset_x;
    var drag_img_offset_y;
    // @todo get this from the server
    var canvas_height = 199;
    var canvas_width = 930;
    var current_file = Drupal.settings.scribble.newestScribble;
    var brush_map = {
      'line': LineBrush,
      'cross': CrossBrush,
      'basic': BasicBrush
    };

    if (current_file != '') {
      // Load the newest image.
      var options = {
        backgroundImage: Drupal.settings.scribble.bgImagePath + '/' + current_file
      };
      $('.scribble-canvas').jqScribble(options);
    }

    // Add the color picker.
    $('.scribble-color-picker').farbtastic(function (color) {
      var rgb = hexToRgb(color);
      rgb_string = 'rgb(' + rgb.join(',') + ')';
      $('.scribble-canvas').data("jqScribble").update({brushColor: rgb_string});
    });

    // Helper to convert hex to rgb codes.
    function hexToRgb(hex) {
      var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex.toString());
      return result ? [
        parseInt(result[1], 16),
        parseInt(result[2], 16),
        parseInt(result[3], 16)
      ] : null;
    }

    // Register the handler for the save action.
    $('.scribble-save').click(function () {
      $(".scribble-canvas").data("jqScribble").save(function (imageData) {
        if(confirm(Drupal.t('You\'re about to save ur changes. Is that cool with you?')) && !$('.scribble-canvas').data('jqScribble').blank) {
          $.post(Drupal.settings.scribble.saveURL, {imagedata: imageData}, function(response) {
            var options = {
              backgroundImage: Drupal.settings.scribble.bgImagePath + '/' + response.file_name
            };
            $('.scribble-canvas').data('jqScribble').update(options);
            current_file = Drupal.settings.scribble.bgImagePath + '/' + response.file_name;
          });
        }
      });
      return false;
    });

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
        backgroundImage: Drupal.settings.scribble.bgImagePath + '/' + current_file
      };
      $('.scribble-canvas').data('jqScribble').update(options);
      console.log($('.scribble-canvas').height());
      return false;
    });

    // Handle image add action.
    $('.scribble-add').click(function () {
      var img_src = prompt(Drupal.t('Enter the URL of the image.'));
      if(img_src !== '' && img_src != null) {
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
          start: function (event, ui) {
            drag_img_offset_x = event.pageX - $('.scribble-add-image').position().left;
            drag_img_offset_y = event.pageY - $('.scribble-add-image').position().top;
          },
          revert: true
        };
        $img.draggable(options);
      }
      return false;
    });

    function addImgStopHandler(event, ui) {
      console.log(event);
      console.log(drag_img_offset_x, drag_img_offset_y);
      // @todo calculate mouse offset from dragged img into x and y
      if (droppedOnCanvas(event.pageX, event.pageY)) {
        var x = event.pageX - $('.scribble-canvas').offset().left - drag_img_offset_x;
        var y = event.pageY - $('.scribble-canvas').offset().top - drag_img_offset_y;
        var data = {
          img_url: $('.scribble-add-image').attr('src'),
          img_width: $('.scribble-add-image').width(),
          img_height: $('.scribble-add-image').height(),
          dst_x: x,
          dst_y: y
        };
        if (current_file == '') {
          data.canvas_width = canvas_width;
          data.canvas_height = canvas_height;
        }
        $.post(Drupal.settings.scribble.addURL, data, function(response) {
          var options = {
            backgroundImage: Drupal.settings.scribble.bgImagePath + '/' + response.file_name
          };
          $('.scribble-canvas').data('jqScribble').update(options);
          current_file = Drupal.settings.scribble.bgImagePath + '/' + response.file_name;
        });
      }
    }

    // Helper function to check if image was dropped within the canvas.
    function droppedOnCanvas(x, y) {
      $canvas = $('.scribble-canvas');
      var border_left = $canvas.offset().left;
      var border_top = $canvas.offset().top;
      var border_right = $canvas.offset().left + $canvas.width();
      var border_bottom = $canvas.offset().top + $canvas.height();
      if (x > border_left && x < border_right && y > border_top && y < border_bottom) {
        return true;
      }
      return false;
    }
  }

})(jQuery);
