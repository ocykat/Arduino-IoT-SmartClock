<?php
    require_once("../global/head.php");
?>
    <!-- Styling -->
    <link rel="stylesheet" href="../../style/global.css">
    <!-- Data update -->
    <script type="text/javascript" src="update.js"></script>
    <!-- Title -->
    <title> Arduino Data </title>
</head>

<body>
<?php
    require_once("../global/navbar.php");
?>
    <div class="container">
        <h1 class="header page-header">
            Arduino Data
        </h1>
        <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/378145/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Temperature&type=line"></iframe>
        <iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/378145/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Humidity&type=line"></iframe>

        <h3 class="page-header">
            Lastest reads
        </h3>

    <table class="table table-striped">
        <thead>
            <tr>
                <th> Time </th>
                <th> Temperature </th>
                <th> Humidity </th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
            <tr>
                <td class="time"></td>
                <td class="temp"></td>
                <td class="rh"></td>
            </tr>
        </tbody>
    </table>

    </div>
</body>

</html>
