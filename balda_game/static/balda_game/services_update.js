/**
 * Created by akhtyamovpavel on 28.05.15.
 */

(function() {
    function loadRating() {
        $.get('/load_best', "json").done(function(data) {
            var ratingList = data.field;
            var newTable = $('<tbody class=rating_table_body>');
            for (var i = 0; i < ratingList.length; ++i) {
                var row = ratingList[i];
                var trow = $('<tr>');
                var place =$('<td>').text(row.place);
                var user = $('<td>');

                user.append($('<a>').attr('href', '/profile/' + row.user).text(row.user));
                var rating = $('<td>').text(row.rating);
                trow.append(place);
                trow.append(user);
                trow.append(rating);
                newTable.append(trow);
            }
            $(".rating_table_body").replaceWith(newTable);
        });
    }

    loadRating();
    setInterval(loadRating, 15000);
})();