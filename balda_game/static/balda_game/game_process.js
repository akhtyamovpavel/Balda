function hideClickedInfo() {
    var values = $('div[class="if_clicked"]');
    console.log(values.length);
    for(var i = 0; i < values.length; ++i) {
        $(values[i]).css('display', 'none');
    }
}
/**
 * Created by akhtyamovpavel on 27.04.15.
 */
$(document).ready(function() {
    hideClickedInfo();

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
});