// var sample = document.getElementById("projectorSoundEffect").;
// sample.play();

// window.onload = function() {
//     document.getElementById("projectorSoundEffect").play();
// }

// var media = document.getElementById("projectorSoundEffect");
// const playPromise = media.play();
// if (playPromise !== null){
//     playPromise.catch(
//         function() { media.play(); })
// }
(function($) {
    // You pass-in jQuery and then alias it with the $-sign
    // So your internal code doesn't change
    if (document.getElementById("#projectorSoundEffect") !== null) {
        $(document).ready(function () {
            $("#projectorSoundEffect").get(0).play();
        });

        $('.slick-next').addClass('projector');
        $('.projector').click = (
            function () {
                console.log('action detected');
                $("#projectorSoundEffect").get(0).play()
            });
    }
})(jQuery);
