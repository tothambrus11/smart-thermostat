import {Component, Input, EventEmitter, OnInit, Output} from '@angular/core';

@Component({
  selector: 'app-switch',
  templateUrl: './switch.component.html',
  styleUrls: ['./switch.component.scss']
})
export class SwitchComponent implements OnInit {
  @Input()
  state!: boolean;

  @Output()
  stateChange = new EventEmitter<boolean>();

  constructor() { }

  ngOnInit(): void {
  }

  changeState() {
    this.state=!this.state;
    this.stateChange.emit(this.state);
  }
}
