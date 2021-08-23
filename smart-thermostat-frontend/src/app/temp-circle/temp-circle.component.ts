import {Component, Input, OnInit} from '@angular/core';
import {TempService} from '../temp.service';

@Component({
  selector: 'app-temp-circle',
  templateUrl: './temp-circle.component.html',
  styleUrls: ['./temp-circle.component.scss']
})
export class TempCircleComponent implements OnInit {

  constructor(public tempService: TempService) {
  }

  ngOnInit(): void {
  }

}
