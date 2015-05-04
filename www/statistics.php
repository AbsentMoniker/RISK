<?php

$arr = array();
$output = array();
unset($arr); // Delete the whole array
unset($output);
exec('./risk_spi_stats --length 135', $output); // Ask the game board for the stats info
//$arr = array('territories' => array(), 'cards' => array()); // Setup structure -> $arr = { 'territories' : {}, 'cards' : {} }

// First 84 bytes are dice stuff
for ($i = 0; $i < 6; $i++) {
  $arr[$i] = array();
  // Dice stuff
  $arr[$i]['dice']   = ord(substr($output[0], $i * 6 + 0, 1))  * 256 + ord(substr($output[0], $i * 6 + 1, 1));
  $arr[$i]['ones']   = ord(substr($output[0], $i * 6 + 2, 1))  * 256 + ord(substr($output[0], $i * 6 + 3, 1));
  $arr[$i]['twos']   = ord(substr($output[0], $i * 6 + 4, 1))  * 256 + ord(substr($output[0], $i * 6 + 5, 1));
  $arr[$i]['threes'] = ord(substr($output[0], $i * 6 + 6, 1))  * 256 + ord(substr($output[0], $i * 6 + 7, 1));
  $arr[$i]['fours']  = ord(substr($output[0], $i * 6 + 8, 1))  * 256 + ord(substr($output[0], $i * 6 + 9, 1));
  $arr[$i]['fives']  = ord(substr($output[0], $i * 6 + 10, 1)) * 256 + ord(substr($output[0], $i * 6 + 11, 1));
  $arr[$i]['sixes']  = ord(substr($output[0], $i * 6 + 12, 1)) * 256 + ord(substr($output[0], $i * 6 + 13, 1));
  // Troops
  $arr[$i]['troops'] = ord(substr($output[0], $i * 6 + 0 + 84, 1))  * 256 + ord(substr($output[0], $i * 6 + 1 + 84, 1));
  $arr[$i]['kills']  = ord(substr($output[0], $i * 6 + 2 + 84, 1))  * 256 + ord(substr($output[0], $i * 6 + 3 + 84, 1));
  $arr[$i]['deaths'] = ord(substr($output[0], $i * 6 + 4 + 84, 1))  * 256 + ord(substr($output[0], $i * 6 + 5 + 84, 1));
  // Territories
  $arr[$i]['territory'] = ord(substr($output[0], $i * 6 + 0 + 120, 1));
  $arr[$i]['turns'] = ord(substr($output[0], $i * 6 + 1 + 120, 1));
}

// Most fought over territory
$arr['territory'] = ord(substr($output[0], 132, 1));
$arr['turns']     = ord(substr($output[0], 133, 1));

$json = json_encode($arr);

// Debug by printing the JSON
//$fp = fopen("/var/www/statistics.json", "w");
//fwrite($fp, $json);
//fclose($fp);

echo $json;

?>
