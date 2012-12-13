<?php

$now = 0xFA00;
$runTime = $now;
$rate = 0xFF;
$ovf = 0xFFFF;
$wait = 0;
$inc = 1;

function addos($x, $y, $ovf) {
	if (abs($x + $y) > $ovf)
		$x = $x - 2*$ovf;
	return $x + $y;
}

function addou($x, $y, $ovf) {
	if (abs($x + $y) > $ovf)
		$x = $x - $ovf;
	return $x + $y;
}

function subos($x, $y, $ovf) {
	if (abs($x - $y) > $ovf)
		$x = $x - 2*$ovf;
	return $x - $y;
}

function subou($x, $y, $ovf) {
	if (abs($x - $y) > $ovf)
		$x = $x - $ovf;
	return $x - $y;
}

for ($i = 0; $i < 0xFFF; $i++) {
	$canRun = subos($now, $runTime, $ovf) >=0;
	/*if (!$wait && !$canRun) {
		echo "Fehler 1 wait = $wait runTime = $runTime now = $now\n";
	}
	if ($wait && $canRun) {
		echo "Fehler 2 wait = $wait runTime = $runTime now = $now\n";
	}*/
	if ($canRun) {
		echo 'now-runTime = '. subos($now, $runTime, $ovf) .' ';
		echo "now = $now ";
		echo "runTime = $runTime ";
		$runTime = addou($runTime, $rate, $ovf);
		$wait = $rate;
		echo "next = $runTime\n";
	}
	$now = addou($now, $inc, $ovf);
	$wait -= $inc;
}

?>