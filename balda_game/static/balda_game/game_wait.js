/**
 * Created by akhtyamovpavel on 27.04.15.
 */
(function() {
    function onWait() {
        $.get('/wait_query').done(function(data) {
            var value = data.game;
            if (value >= 0) {
                window.location.replace("/game/"+value.toString());
            }
        });
    }

    onWait();
    setInterval(onWait, 5000)
})();