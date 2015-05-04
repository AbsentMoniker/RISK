<?php

  $player = $_REQUEST["player"];
  $cards = $_REQUEST["cards"];
  //echo $player;
  //echo $cards[0];
  //echo $cards[1];
  //echo $cards[2];
  //exec('/home/pi/risk/risk_spi_cards', $output);
  exec('./risk_spi_cards -p '.$player.' -0 '.$cards[0].' -1 '.$cards[1].' -2 '.$cards[2], $output); // Ask the game board for the current game state
  echo var_dump($output);

?>
