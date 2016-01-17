/**
 * Created by akhtyamovpavel on 27.04.15.
 */
$(document).ready(function() {
    function onWait() {
        $.get('/wait_query').done(function(data) {
            var value = data.game;
            if (value >= 0) {
                window.location.replace("/game/"+value.toString());
            }
        });
    }

    function cancelGameRequest() {
        $.get('/cancel_game_request').done(function(data) {
            var isGameCancelled = data.isGameCancelled;
            if (isGameCancelled) {
                window.location.replace("/");
            }
        });
    }

    $("#quit").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }
        cancelGameRequest();
    });

    $("#play-with-bot").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }

        $.get('/play_with_bot/').done(function(data) {
            var gameId = data.game;
            window.location.replace("/game/"+gameId.toString());
        });
    });

    onWait();
    setInterval(onWait, 100000);

    window.addEventListener("unload", function(e) {
        cancelGameRequest();
    });
});
