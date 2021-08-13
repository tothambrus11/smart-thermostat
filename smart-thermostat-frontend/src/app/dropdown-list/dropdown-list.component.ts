import {Component, Input, OnInit, Output, EventEmitter} from '@angular/core';

@Component({
  selector: 'app-dropdown-list',
  templateUrl: './dropdown-list.component.html',
  styleUrls: ['./dropdown-list.component.scss']
})
export class DropdownListComponent implements OnInit {
  @Input()
  items!: string[];

  @Input()
  selected!: number;

  @Output()
  selectedChange = new EventEmitter<number>();

  hidden = true;

  constructor() {
  }

  ngOnInit(): void {
    this.selected ||= 0;
  }

  select(index: number) {
    if (this.selected != index) {
      this.selected = index;
      this.selectedChange.emit(this.selected);
    }
  }
}
