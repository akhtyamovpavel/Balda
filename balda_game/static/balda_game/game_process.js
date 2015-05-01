
/**
 * Created by akhtyamovpavel on 27.04.15.
 */
$(document).ready(function() {


    var game_id = $(location).attr('pathname').split('/')[2];

    $(".cell").click(function(){
        var flagValue = $($($(this).parent()).children()[1]);
        if (flagValue.text() == 'true') {
            $(this).css('display', 'none');
            flagValue.text('false');
        } else {
            $(this).css('display', 'block');
            flagValue.text('true');
        }

    });

    function onWait() {
        $.get('/get_field/' + game_id.toString()).done(function(data) {
            var value = data.is_your_move;
            // TODO know player order
            if (value == false) {
                $(".game_field").css('pointer-events', 'none');
            } else {
                $(".game_field").css('pointer-events', 'auto');
            }
            var table = $("#field_up");

            var cell_values = jQuery.parseJSON(data.field);
            for (var i = 0; i < cell_values.length; ++i) {
                var current_cell = cell_values[i];
                var height_level = current_cell.height_level;
                var width_level = current_cell.width_level;
                var letter = current_cell.letter;
                var cell_state = current_cell.cell_state;

                var cellDOMObject = $($($($(table).children()[0]).children()[height_level]).children()[width_level]);
                $($(cellDOMObject).children()[0]).text(letter);
                $($(cellDOMObject).children()[1]).text(cell_state.toString());
            }
        });


    }

    onWait();
    setInterval(onWait, 100000);
});