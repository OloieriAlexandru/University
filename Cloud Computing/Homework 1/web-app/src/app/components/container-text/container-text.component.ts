import { Component, Input } from '@angular/core';

@Component({
  selector: 'app-container-text',
  templateUrl: './container-text.component.html',
  styleUrls: ['./container-text.component.scss'],
})
export class ContainerTextComponent {
  @Input() textToDisplay: String = null;

  constructor() {}
}
