set nsigs [ gtkwave::getNumFacs ]
set sigs [list]
set filter [list led.blue led.green led.red msp430.intr_num\[7:0\] ]

for {set i 0} {$i < $nsigs} {incr i} {
	set name [ gtkwave::getFacName $i ] 
	puts $name
	
	for {set j 0} {$j < [llength $filter]} {incr j} {
		set name2 [lindex $filter $j]
		if {$name2 == $name} {
			lappend sigs $name
		}
	}
}
set num_added [ gtkwave::addSignalsFromList $sigs ]