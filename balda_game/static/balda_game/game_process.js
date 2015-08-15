
/**
 * Created by akhtyamovpavel on 27.04.15.
 */
$(document).ready(function() {

    // sending a csrftoken with every ajax request
    function csrfSafeMethod(method) {
        // these HTTP methods do not require CSRF protection
        return (/^(GET|HEAD|OPTIONS|TRACE)$/.test(method));
    }


    $.ajaxSetup({
        crossDomain: false, // obviates need for sameOrigin test
        beforeSend: function(xhr, settings) {
            if (!csrfSafeMethod(settings.type)) {
                xhr.setRequestHeader("X-CSRFToken", $.cookie('csrftoken'));
            }
        }
    });

    var timer = null;
    function simple_timer(sec, block, direction) {
        var time    = sec;
        direction   = direction || false;

        var hour    = parseInt(time / 3600);
        if ( hour < 1 ) hour = 0;
        time = parseInt(time - hour * 3600);
        if ( hour < 10 ) hour = '0'+hour;

        var minutes = parseInt(time / 60);
        if ( minutes < 1 ) minutes = 0;
        time = parseInt(time - minutes * 60);
        if ( minutes < 10 ) minutes = '0'+minutes;

        var seconds = time;
        if ( seconds < 10 ) seconds = '0'+seconds;

        block.innerHTML = minutes+':'+seconds;

        if ( direction ) {
            sec++;

            timer = setTimeout(function(){ simple_timer(sec, block, direction); }, 1000);
        } else {
            sec--;

            if ( sec > 0 ) {
                timer = setTimeout(function(){ simple_timer(sec, block, direction); }, 1000);
            } else {

                $.get('/give_up/' + game_id.toString(), "json").done(function(data2) {
                   var data = $.parseJSON(data2);
                    performData(data);
                });

            }
        }
    }

    var pinnedWeightLevel = -1;
    var pinnedHeightLevel = -1;
    var isPinned = false;

    var isYourMove = false;


    var game_id = $(location).attr('pathname').split('/')[2];


    var currentWord = [];
    var currentWeightLevelWord = [];
    var currentHeightLevelWord = [];
    var currentWordStr = "";
    var pinnedLetter;


    function resetWord() {
        currentWord = [];
        currentHeightLevelWord = [];
        currentWeightLevelWord = [];
        isPinned = false;
        pinnedWeightLevel = -1;
        pinnedHeightLevel = -1;
        currentWordStr = "";
        $($(".current_word")[0]).val(currentWordStr);
    }

    function addLetter(heightLevel, widthLevel, letter) {
        currentWord[currentWord.length] = letter.trim();
        currentWeightLevelWord[currentWeightLevelWord.length] = widthLevel;
        currentHeightLevelWord[currentHeightLevelWord.length] = heightLevel;
        currentWordStr += letter.trim();
        $($(".current_word")[0]).val(currentWordStr);
    }

    function getCellElement(currentHeight, currentWidth) {
        if (currentHeight < 0 && currentHeight >= 5 || currentWidth < 0 && currentWidth >= 5) {
            return;
        }
        var table = $("#field_up");
        return $($($($(table).children()[0]).children()[currentHeight]).children()[currentWidth]);
    }

    function lockTable() {
        var tableStruct = $("#field_up");
        $($(tableStruct)[0]).css('pointer-events', 'none');
    }

    function unlockTable() {
        var tableStruct = $("#field_up");
        $($(tableStruct)[0]).css('pointer-events', 'auto');
    }

    $("#reset").click(function(e) {
        if (!isYourMove) {
            return;
        }
        if (e.preventDefault()) {
            e.preventDefault();
        }
        if (isPinned) {
            var cell = getCellElement(pinnedHeightLevel, pinnedWeightLevel);
            $($($(cell)[0]).children()[0]).text('.');
            $($($(cell)[0]).children()[1]).text('2');
        }
        resetWord();
        $($(".current_word")[0]).val(currentWordStr);
    });



    $(".cell").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }
        if (!isYourMove) {
            return;
        }
        var parent = $($(this).parent())[0];
        var parentParent = $($(parent).parent())[0];
        if (isPinned) {
            var indexWeightLevel = $($(parentParent).children()).index(parent);
            var indexHeightLevel = $($($($(parentParent).parent())[0]).children()).index(parentParent);
            addLetter(indexHeightLevel, indexWeightLevel, $(this).text());
        } else {
            if ($($($(this).parent()).children()[1]).text() != '2') {
                return;
            }
            pinnedWeightLevel = $($(parentParent).children()).index(parent);
            pinnedHeightLevel = $($($($(parentParent).parent())[0]).children()).index(parentParent);
            isPinned = true;
            $("#letters_panel").css('display', 'block');
            lockTable();
        }
    });

    $(".letter_button").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }
        if (!isYourMove) {
            return;
        }
        if (!isPinned) {
            $(this).display('css', 'none');
            return;
        }

        var cell = getCellElement(pinnedHeightLevel, pinnedWeightLevel);


        $($($(cell)[0]).children()[0]).text($(this).text());

        pinnedLetter = ($(this).text()).trim();
        $($($(cell)[0]).children()[1]).text('1');

        $($($(this).parent())[0]).css('display', 'none');
        unlockTable();
        isPinned = true;
    });

    function performData(data) {
        var value = data.is_your_move;
        // TODO know player order

        var timer_block = document.getElementsByClassName('times')[0];
        if (data.bot === true) {
            timer = null;
            simple_timer(60, timer_block, false);
        } else {
            if (value == false) {
                isYourMove = false;
                clearTimeout(timer);
                timer = null;
                timer_block.innerHTML = "01:00";
            } else {
                isYourMove = true;
                if (timer == null) {
                    simple_timer(60, timer_block, false);
                }
            }
        }

        var score1 = data.score1;
        var score2 = data.score2;
        var player1 = data.player1;
        var player2 = data.player2;
        var action = data.action;
        var firstWords = data.words1;
        var secondWords = data.words2;

        if (action == 'reset') {
            resetWord();
        } else if (action == 'ok') {
            resetWord();
        }

        $(".user_id_first").text(player1);
        $(".user_id_second").text(player2);
        $(".score_first").text("Score:" + score1);
        $(".score_second").text("Score: " + score2);

        var tag = $('<ul class=words_first>');
        for (var i = 0; i < firstWords.length; ++i) {
            tag.append($('<li>').text(firstWords[i]));
        }
        $(".words_first").replaceWith(tag);


        var tag2 = $('<ul class=words_second>');
        for (var i = 0; i < secondWords.length; ++i) {
            tag2.append($('<li>').text(secondWords[i]));
        }
        $(".words_second").replaceWith(tag2);
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
        if (action == 'end') {
            alert(player1 + " " + score1 +  " : " + score2 + " " + player2);
            window.location.replace('/')
        }
    }

    $("#game_space").submit(function (e) {

        if (e) {
            e.preventDefault();
        }
        if (!isYourMove) {
            return;
        }

        $.ajax({
            type:"POST",
            url: '/commit_word/' + game_id.toString() + '/',
            data: {
                pinned_height: pinnedHeightLevel,
                pinned_width: pinnedWeightLevel,
                pinned_letter: pinnedLetter,
                word: currentWordStr,
                'heights[]' : currentHeightLevelWord,
                'widths[]' : currentWeightLevelWord
            },
            dataType: "json"
         }).done(function (data2) {
            performData(data2);
        });
    });





    function onWait() {
        $.get('/get_field/' + game_id.toString(), "json").done(function(data2) {
            var data = $.parseJSON(data2);
            performData(data);
        });

    }


    onWait();
    setInterval(onWait, 4000);
});


