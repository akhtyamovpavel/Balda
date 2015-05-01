
/**
 * Created by akhtyamovpavel on 27.04.15.
 */
$(document).ready(function() {

    var pinnedWeightLevel = 0;
    var pinnedHeightLevel = 0;
    var isPinned = false;

    var game_id = $(location).attr('pathname').split('/')[2];

    function getCellElement(x, y) {
        if (x < 0 && x >= 5 || y < 0 && y >= 5) {
            return;
        }
        var table = $("#field_up");
        return $($($($(table).children()[0]).children()[x]).children()[y]);
    }

    function lockTable() {
        var tableStruct = $("#field_up");
        $($(tableStruct)[0]).css('pointer-events', 'none');
    }

    function unlockTable() {
        var tableStruct = $("#field_up");
        $($(tableStruct)[0]).css('pointer-events', 'auto');
    }

    $(".cell").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }
        if ($($($(this).parent()).children()[1]).text() != '2') {
            return;
        }
        if (isPinned) {
            return;
        }
        var parent = $($(this).parent())[0];
        var parentParent = $($(parent).parent())[0];
        pinnedHeightLevel = $($(parentParent).children()).index(parent);
        pinnedWeightLevel = $($($($(parentParent).parent())[0]).children()).index(parentParent);
        isPinned = true;
        $("#letters_panel").css('display', 'block');
        lockTable();
    });

    $(".letter_button").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }
        if (!isPinned) {
            $(this).display('css', 'none');
            return;
        }

        var cell = getCellElement(pinnedWeightLevel, pinnedHeightLevel);

        $($($(cell)[0]).children()[0]).text($(this).text());
        $($($(cell)[0]).children()[1]).text('1');

        $($($(this).parent())[0]).css('display', 'none');
        unlockTable();
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
                var heightLevel = current_cell.height_level;
                var widthLevel = current_cell.width_level;
                if (isPinned) {
                    if (heightLevel == pinnedHeightLevel && widthLevel == pinnedWeightLevel) {
                        continue;
                    }
                }
                var letter = current_cell.letter;
                var cell_state = current_cell.cell_state;
                if (!value && cell_state == 1) { // some check, may be from server info
                    continue;
                }
                var cellDOMObject = $($($($(table).children()[0]).children()[heightLevel]).children()[widthLevel]);
                $($(cellDOMObject).children()[0]).text(letter);
                $($(cellDOMObject).children()[1]).text(cell_state.toString());
            }
        });

    }






    onWait();
    setInterval(onWait, 100000);
});