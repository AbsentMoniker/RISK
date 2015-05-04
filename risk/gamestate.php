<?php

$a = 0;
$arr = array();
$prev = "";
$output = array();
while (1) {
  //exec('./risk_spi_gamestate --length 1', $output);
  //usleep(100000); // 100ms
  unset($output);
  exec('/home/pi/risk/risk_spi_gamestate --length 87', $output); // Ask the game board for the current game state
  if ($output != $prev) {
    //echo "Gamestate changed!".PHP_EOL.bin2hex($output[0]).PHP_EOL.bin2hex($prev[0]).PHP_EOL;
    //echo $output[0];
    //for ($i = 0; $i < strlen($output[0]); $i++) {
    //  echo substr($output[0], $i, 1);
    //  echo ' ';
    //}
    //echo "\n";

    unset($arr); // Delete the whole array
    $arr = array('territories' => array(), 'cards' => array()); // Setup structure -> $arr = { 'territories' : {}, 'cards' : {} }

    // Interpret the first 42 bytes as who owns each territory 0-42 (bytes should be 0-5 or 255)
    for ($i = 0; $i < 42; $i++) {
      $arr['territories'][$i] = ord(substr($output[0], $i, 1)); // Store (TerritoryId => OwnerId) -> $arr = { 'territories' : { 0 : 1, 1 : 3, ... }, 'cards' : {} }
    }

    // Interpret the next 44 bytes as who owns each card 0-44 (bytes should be 0-5 or 255)
    for ($i = 0; $i < 44; $i++) {
      $arr['cards'][$i] = ord(substr($output[0], $i + 42, 1));
      //array_push($arr['cards'][ord(substr($output[0], $i, 1))], $i - 44); // Append (OwnerId => (..., CardId))
    }

    $json = json_encode($arr);

    // Debug by printing the JSON
    //echo $json."\n";
    $fp = fopen("/var/www/gamestate.json", "w");
    fwrite($fp, $json);
    fclose($fp);

    usleep(100000); // Sleep for 100ms (0.1s)
  }
  $prev = $output;

  $a = $a + 1;
  if ($a == 10) {
    $a = 0;
    unset($arr); // Delete the whole array
    unset($output);
    exec('/var/www/risk_spi_stats --length 135', $output); // Ask the game board for the stats $

    // First 84 bytes are dice stuff
    for ($i = 0; $i < 6; $i++) {
      $arr[$i] = array();
      // Dice stuff
      $arr[$i]['dice']   = ord(substr($output[0], $i * 14 + 0, 1))  * 256 + ord(substr($output[0], $i * 14 + 1, 1));
      $arr[$i]['ones']   = ord(substr($output[0], $i * 14 + 2, 1))  * 256 + ord(substr($output[0], $i * 14 + 3, 1));
      $arr[$i]['twos']   = ord(substr($output[0], $i * 14 + 4, 1))  * 256 + ord(substr($output[0], $i * 14 + 5, 1));
      $arr[$i]['threes'] = ord(substr($output[0], $i * 14 + 6, 1))  * 256 + ord(substr($output[0], $i * 14 + 7, 1));
      $arr[$i]['fours']  = ord(substr($output[0], $i * 14 + 8, 1))  * 256 + ord(substr($output[0], $i * 14 + 9, 1));
      $arr[$i]['fives']  = ord(substr($output[0], $i * 14 + 10, 1)) * 256 + ord(substr($output[0], $i * 14 + 11, 1));
      $arr[$i]['sixes']  = ord(substr($output[0], $i * 14 + 12, 1)) * 256 + ord(substr($output[0], $i * 14 + 13, 1));
      // Troops
      $arr[$i]['troops'] = ord(substr($output[0], $i * 6 + 0 + 84, 1))  * 256 + ord(substr($output[0], $i * 6 + 1 + 84, 1));
      $arr[$i]['kills']  = ord(substr($output[0], $i * 6 + 2 + 84, 1))  * 256 + ord(substr($output[0], $i * 6 + 3 + 84, 1));
      $arr[$i]['deaths'] = ord(substr($output[0], $i * 6 + 4 + 84, 1))  * 256 + ord(substr($output[0], $i * 6 + 5 + 84, 1));
      // Territories
      $arr[$i]['territory'] = ord(substr($output[0], $i * 2 + 0 + 120, 1));
      $arr[$i]['turns'] = ord(substr($output[0], $i * 2 + 1 + 120, 1));
    }

    // Most fought over territory
    $arr['territory'] = ord(substr($output[0], 132, 1));
    $arr['turns']     = ord(substr($output[0], 133, 1));

    $json = json_encode($arr);

    // Debug by printing the JSON
    $fp = fopen("/var/www/statistics.json", "w");
    fwrite($fp, $json);
    fclose($fp);

  }
}

?>
