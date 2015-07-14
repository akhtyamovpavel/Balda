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

    $("#quit").click(function(e) {
        if (e.preventDefault()) {
            e.preventDefault();
        }

        $.get('/cancel_game_request').done(function(data) {
            var isGameCancelled = data.isGameCancelled;
            if (isGameCancelled) {
                window.location.replace("/");
            }
        });
    });

    onWait();
    setInterval(onWait, 100000);
});
