import {Component, EventEmitter, Input, OnInit, Output} from '@angular/core';
import {StoredInterval} from '../interval';

@Component({
  selector: 'app-interval-card',
  templateUrl: './interval-card.component.html',
  styleUrls: ['./interval-card.component.scss']
})
export class IntervalCardComponent implements OnInit {

  @Input()
  currentInterval!: StoredInterval;

  @Input()
  allIntervals!: StoredInterval[];

  selectedLmiContainer = false;


  @Output()
  up = new EventEmitter<void>();

  @Output()
  down = new EventEmitter<void>();

  constructor() { }

  ngOnInit(): void {
  }


}
