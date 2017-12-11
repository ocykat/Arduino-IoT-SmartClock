<?php
    $url = "https://api.weatherbit.io/v2.0/current?city=HoChiMinhCity&key=f3dfe6da9513476399d27d61f3658114";

    $json = file_get_contents($url);
    $obj = json_decode($json, TRUE);
?>
