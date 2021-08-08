import {Component, Input, OnInit, Output, EventEmitter} from '@angular/core';

@Component({
  selector: 'app-temp-input',
  templateUrl: './temp-input.component.html',
  styleUrls: ['./temp-input.component.scss']
})
export class TempInputComponent implements OnInit {

  @Input()
  value?:number

  @Output()
  valueChange: EventEmitter<number> = new EventEmitter()

  constructor() { }

  ngOnInit(): void {
  }

}
