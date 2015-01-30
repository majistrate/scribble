/**
 * @file
 * Contains javascript code for the scribble module.
 */

Drupal.scribble = Drupal.scribble || {};

(function($) {

  Drupal.scribble.dialog_base_options = {
    draggable: true,
    autoOpen: false,
    resizable: false,
    hide: "explode"
  };
  Drupal.scribble.$draw_canvas = null;

  /**
   * Behavior for the toolbar buttons and modals.
   */
  Drupal.behaviors.scribbleBlackboardToolbar = {};
  Drupal.behaviors.scribbleBlackboardToolbar.attach = function (context, settings) {
    // Get the canvas into 'global' variable as used in other behaviors as well.
    Drupal.scribble.$draw_canvas = $('.scribble-canvas');

    var $save_btn = $('.scribble-save');
    var $image_btn = $('.scribble-add-image');
    var $clear_btn = $('.scribble-clear');

    var dialog_base_options = {
      draggable: true,
      autoOpen: false,
      resizable: false,
      hide: "explode"
    };

    // Initialize the toolbar buttons.
    $save_btn.button({
      icons: {
        primary: "ui-icon-disk"
      }
    });
    $image_btn.button({
      icons: {
        primary: "ui-icon-image"
      }
    });
    $clear_btn.button({
      icons: {
        primary: "ui-icon-trash"
      }
    });

    // Enable saving after only after first click.
    Drupal.scribble.$draw_canvas.click(function () {
      unchanged = false;
    });

    // Initialize the toolbar tabs.
    $('.scribble-toolbar').tabs({selected: 0});
  };

  Drupal.behaviors.scribbleImageInjection = {};
  Drupal.behaviors.scribbleImageInjection.attach = function (context, settings) {
    var drag_img_offset_x;
    var drag_img_offset_y;
    var $add_img_container = $('.scribble-add-img-modal');
    var $add_img;
    var img_dialog_width = 0;
    var add_img_height;
    var add_img_width;
    var $scribble_add_btn = $('.scribble-add');
    var $web_src_txt = $('#img-src-txt');

    // Handle image add action.
    $scribble_add_btn.click(function () {
      var img_src = $web_src_txt.val();
      if(img_src !== '') {
        validatedImageLoad(img_src);
      }
      else {
        $web_src_txt.addClass('ui-state-error');
      }
    });

    function validatedImageLoad(URL) {
      var $load_img = $(new Image());
      $load_img.error(function() {
        $web_src_txt.addClass('ui-state-error');
      })
        .load(function() {
          loadAddImageDialog($(this));
        });
      $load_img.attr('src', URL);
    }

    function loadAddImageDialog($img) {
      $img.addClass('scribble-add-img');
      $add_img_container.html($img);
      $('#img-src-txt').removeClass('ui-state-error');
      img_dialog_width = (img_dialog_width != 0) ? img_dialog_width: $img.width();
      $add_img_container.dialog({
        draggable: true,
        title: Drupal.t('Drag the image on the blackboard in order to add it.'),
        autoOpen: false,
        resizable: false,
        width: img_dialog_width,
        hide: "explode"
      });
      var options = {
        stop: addImgDropHandler,
        start: function (event, ui) {
          // Set the image to be added in var to use it in the drop handler.
          $add_img = $('.scribble-add-img');
          // Set vars for mouse offset left upper corner of the image.
          drag_img_offset_x = event.pageX - $add_img.offset().left;
          drag_img_offset_y = event.pageY - $add_img.offset().top;
          add_img_width = $add_img.width();
          add_img_height = $add_img.height();
          $add_img_container.dialog('close');
        },
        revert: true,
        helper: 'clone',
        appendTo: 'body',
        scroll: false,
        zIndex: 1500
      };
      $img.draggable(options);
      $add_img_container.dialog('open');
    }

    // Fires once the dragged image is dropped on the draw canvas.
    function addImgDropHandler(event, ui) {
      if (droppedOnCanvas(event.pageX, event.pageY)) {
        // Gather data for image merge.
        var x = event.pageX - Drupal.scribble.$draw_canvas.offset().left - drag_img_offset_x;
        var y = event.pageY - Drupal.scribble.$draw_canvas.offset().top - drag_img_offset_y;
        var data = {
          img_url: $add_img.attr('src'),
          img_width: add_img_width,
          img_height: add_img_height,
          dst_x: x,
          dst_y: y,
          scribble_id: Drupal.settings.scribble_info.scribbleId
        };
        // Do AJAX post that merges the images and saves a new image.
        $.post(Drupal.settings.scribble.addURL, data, function(response) {
          // Store the latest filename.
          current_file = response.file_name;
          // Update the background of the canvas with the new image.
          $('.scribble-canvas-wrapper').css('background-image', 'url("' + dir_path + '/' + current_file + '")');
          Drupal.scribble.$draw_canvas.data("jqScribble").clear();
        });
      }
    }

    // Helper function to check if image was dropped within the canvas.
    function droppedOnCanvas(x, y) {
      var $canvas = Drupal.scribble.$draw_canvas;
      var border_left = $canvas.offset().left;
      var border_top = $canvas.offset().top;
      var border_right = $canvas.offset().left + $canvas.width();
      var border_bottom = $canvas.offset().top + $canvas.height();
      return x > border_left && x < border_right && y > border_top && y < border_bottom;
    }
  };

  // Register new AJAX command that is used in the server callback specified in
  // the image upload submit button within the blackboard form.
  Drupal.ajax.prototype.commands.scribbleAddImage = Drupal.scribble.scribbleAddImage;

  /**
   * AJAX command callback.
   *
   * @todo describe code.
   */
  Drupal.scribble.scribbleAddImage = function (ajax, response, status) {
    var $load_img = $(new Image());
    $load_img.error(function() {
      // @todo display error message in messages container
    })
      .load(function() {
        loadAddImageDialog($(this));
      });
    $load_img.attr('src', URL);
  }

  /**
   * Behavior for the brush color and style selection.
   */
  Drupal.behaviors.scribbleBrushOptions = {};
  Drupal.behaviors.scribbleBrushOptions.attach = function (context, settings) {
    var $brush_btns = $('.scribble-brush-button');
    var $color_btn = $('.scribble-color-btn');
    var $size_display = $('.scribble-brush-size-display');
    var $brush_size = $('.scribble-brush-size');
    var $color_picker = $('.scribble-color-picker');

    $color_btn
      .button({
        icons: {
          primary: "ui-icon-pencil"
        }
      })
      .click(function () {
        $('.scribble-color-picker').dialog('open');
      });

    $('.scribble-color-display').click(function (event) {
      $('.scribble-color-picker').dialog('open');
      event.stopPropagation();
    });

    // Register brush handlers
    $brush_btns.click(function () {
      $brush_btns.removeClass('active-brush');
      $(this).addClass('active-brush');
      // Update the scribble to use the selected brush, the buttons id attribute
      // is the JS brush class name.
      Drupal.scribble.$draw_canvas.data("jqScribble").update({brush: eval($(this).data('brush'))});
    });

    // Add the color picker.
    $color_picker
      .ColorPicker({
        flat: true,
        onChange: function(hsb, hex, rgb) {
          var html_color = '#' + hex;
          Drupal.scribble.$draw_canvas.data("jqScribble").update({brushColor: html_color});
          $('.scribble-color-display').css('background-color', html_color);
        }
      })
      .dialog({
        draggable: true,
        title: Drupal.t('Choose your color'),
        autoOpen: false,
        resizable: false,
        width: 356,
        hide: "explode"
      });

    $('.scribble-blackboard-wrapper').mousedown(function () {
      if ($color_picker.dialog('isOpen')) {
        $('.scribble-color-picker').dialog('close');
      }
    });

    // Initialize brush size slider.
    var options = {
      stop: function (event, ui) {
        Drupal.scribble.$draw_canvas.data("jqScribble").update({brushSize: ui.value});
      },
      slide: function (event, ui) {
        $size_display.text(ui.value);
      },
      min: 1
    };
    $brush_size.slider(options);
    $size_display.text('1');
  };

  /**
   * Behavior for scribble module.
   */
  Drupal.behaviors.createBlackboard = {};
  Drupal.behaviors.createBlackboard.attach = function (context, settings) {

    var drag_img_offset_x;
    var drag_img_offset_y;
    var current_file = Drupal.settings.scribble_info.newestScribble;
    var dir_path = Drupal.settings.scribble.bgImagePath + '/' + Drupal.settings.scribble_info.scribbleId;
    var $add_img_container = $('.scribble-add-img-modal');
    var $save_btn = $('.scribble-save');
    var $add_img;
    var unchanged = true;
    var img_dialog_width = 0;
    var add_img_height;
    var add_img_width;

    if (current_file != '' && current_file !== undefined && current_file !== null) {
      // Load the newest image as background in the wrapper element of canvas.
      $('.scribble-canvas-wrapper').css('background-image', 'url("' + dir_path + '/' + current_file + '")');
    }
    // Initialize drawing canvas.
    Drupal.scribble.$draw_canvas.jqScribble({fillOnClear: false});

    // Register the handler for the save action.
    $save_btn.click(function () {
      if (!unchanged) {
        Drupal.scribble.$draw_canvas.data("jqScribble").save(function (imageData) {
          if(confirm(Drupal.t('You\'re about to save your changes. Is that cool with you?')) && !$draw_canvas.data('jqScribble').blank) {
            var post_data = {
              imagedata: imageData,
              scribble_id: Drupal.settings.scribble_info.scribbleId
            };
            $.post(Drupal.settings.scribble.saveURL, post_data, function(response) {
              current_file = response.file_name;
              $('.scribble-canvas-wrapper').css('background-image', 'url("' + dir_path + '/' + current_file + '")');
              Drupal.scribble.$draw_canvas.data("jqScribble").clear();
            });
          }
        });
      }
    });

    // Reset to last saved background image.
    $('.scribble-clear').click(function () {
      Drupal.scribble.$draw_canvas.data('jqScribble').clear();
      unchanged = true;
    });
  }

})(jQuery);
