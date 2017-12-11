<?php
    require_once("../global/head.php");
?>
    <!-- Styling -->
    <link rel="stylesheet" href="../../style/global.css">
    <!-- Title -->
    <title> Real-time temperature </title>
</head>

<body>
    <?php
        require_once("../global/navbar.php");
        require_once("action.php");
    ?>

    <div class="container">
        <h1 class="header page-header">
            Current Weather
            <small> Ho Chi Minh City - Vietnam </small>
        </h1>
        <div id="data"><p>Temp: <?php echo $obj["data"][0]["temp"]; ?></p><p>RH: <?php echo $obj["data"][0]["rh"]; ?></p></div>
        <br />
        <p class="lead"> Data collected from <a href="weatherbit.io"> weatherbit.io </a> </p>
    </div>
</body>

</html>
