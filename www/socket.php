<?php
  exec('echo "PHP test" | /home/pi/ece477/echo_client');
/*
  //$socket = stream_socket_client("unix:///home/pi/ece477/echo_socket", $errno, $errstr, 5);
  $socket = fsockopen("unix:///home/pi/ece477/echo_socket", -1, $errno, $errstr, 5);

  if (!$socket) {
    echo "$errstr ($errno)\n";
    die(-1);
  }

  //fwrite($socket, "fdhsahfsdjahfksdahfkjsdahfkjsdahfkjsdhafkjsdahfkjsdhafkhsdakfhdsakjfsfdaafsdfsdsfdsafdfsdafsdfffffff");
  fwrite($socket, "PHP: Socket write test");
  echo "Data written";

  fclose($socket);
*/
?>
