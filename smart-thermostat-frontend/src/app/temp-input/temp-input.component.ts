import {Component, Input, OnInit, Output, EventEmitter} from '@angular/core';

@Component({
  selector: 'app-temp-input',
  templateUrl: './temp-input.component.html',
  styleUrls: ['./temp-input.component.scss']
})
export class TempInputComponent implements OnInit {

  @Input()
  value?: number;

  @Input()
  placeholder = '';

  @Output()
  valueChange: EventEmitter<number> = new EventEmitter();

  @Output()
  changed: EventEmitter<number> = new EventEmitter<number>();

  constructor() {
  }

  ngOnInit(): void {
  }

  plus(): void {
    this.value ||= 0;
    this.value *= 10;
    this.value++;
    this.value = Math.round(this.value);
    this.value /= 10;
    this.valueChange.emit(this.value);
    this.changed.emit(this.value);
  }

  minus(): void {
    this.value ||= 0;
    this.value *= 10;
    this.value--;
    this.value = Math.round(this.value);
    this.value /= 10;
    this.valueChange.emit(this.value);
    this.changed.emit(this.value);
  }
}
