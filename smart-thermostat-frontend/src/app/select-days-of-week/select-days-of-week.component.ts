import {Component, Input, OnInit, Output, EventEmitter} from '@angular/core';

export const daysLocalized = [
  'hétfő', 'kedd', 'szerda', 'csütörtök', 'péntek', 'szombat', 'vasárnap'
];
export const daysShostLocalized = [
  'H', 'K', 'SZE', 'CS', 'P', 'SZO', 'V'
];

let daysWithNumber = daysShostLocalized.map((day, index) => {
  return {day, index};
});

function nthBit(number: number, n: number) {
  return (number >> n) & 1;
}

function numberToBits(number: number, arrayToAdd: boolean[]) {
  for (let i = 0; i < 7; i++) {
    arrayToAdd.unshift(!!nthBit(number, i));
  }
}

function bitsToNumber(bits: boolean[]) {
  let number = 0;
  for (let bit of bits) {
    number <<= 1;
    if (bit) {
      number++;
    }
  }
  return number;
}

@Component({
  selector: 'app-select-days-of-week',
  templateUrl: './select-days-of-week.component.html',
  styleUrls: ['./select-days-of-week.component.scss']
})
export class SelectDaysOfWeekComponent implements OnInit {
  daysWithNumber = daysWithNumber;

  public values: boolean[] = [];

  @Input()
  value!: number;

  @Output()
  valueChange = new EventEmitter<number>();

  constructor() {
  }

  ngOnInit(): void {
    numberToBits(this.value, this.values);
  }

  onChange(){
    this.value = bitsToNumber(this.values);
    this.valueChange.emit(this.value);
    console.log(this.value)
  }

}
