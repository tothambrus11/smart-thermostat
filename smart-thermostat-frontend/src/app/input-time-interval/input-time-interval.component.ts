import {Component, OnInit, EventEmitter, Input, Output} from '@angular/core';

@Component({
  selector: 'app-input-time-interval',
  templateUrl: './input-time-interval.component.html',
  styleUrls: ['./input-time-interval.component.scss']
})
export class InputTimeIntervalComponent implements OnInit {

  constructor() { }

  @Input()
  time1 = '00:00';

  @Output()
  time1Change = new EventEmitter<string>();

  @Input()
  time2 = '00:00';

  @Output()
  time2Change = new EventEmitter<string>();

  ngOnInit(): void {

  }

}
