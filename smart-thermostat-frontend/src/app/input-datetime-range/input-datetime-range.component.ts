import {Component, EventEmitter, Input, OnInit, Output} from '@angular/core';

@Component({
  selector: 'app-input-datetime-range',
  templateUrl: './input-datetime-range.component.html',
  styleUrls: ['./input-datetime-range.component.scss']
})
export class InputDatetimeRangeComponent implements OnInit {
  @Input()
  dateTime1: string = "";

  @Output()
  dateTime1Change: EventEmitter<string> = new EventEmitter<string>();

  @Input()
  dateTime2: string = "";

  @Output()
  dateTime2Change: EventEmitter<string> = new EventEmitter<string>();

  constructor() {
  }

  ngOnInit(): void {
  }

}
