import { ComponentFixture, TestBed } from '@angular/core/testing';

import { TempCircleComponent } from './temp-circle.component';

describe('TempCircleComponent', () => {
  let component: TempCircleComponent;
  let fixture: ComponentFixture<TempCircleComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ TempCircleComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(TempCircleComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
