import * as React from "react";
import TreeView from "@mui/lab/TreeView";
import { styled } from "@mui/material/styles";
import ExpandMoreIcon from "@mui/icons-material/ExpandMore";
import ChevronRightIcon from "@mui/icons-material/ChevronRight";
import TreeItem, { treeItemClasses } from "@mui/lab/TreeItem";

import Button from '@mui/material/Button';
import Switch from '@mui/material/Switch';
import FormGroup from '@mui/material/FormGroup';
import FormControlLabel from '@mui/material/FormControlLabel';
import FormControl from '@mui/material/FormControl';
import FormLabel from '@mui/material/FormLabel';

type StyledTreeItemProps = {
  rootNode ?: boolean;
};

const StyledTreeItem = styled(TreeItem)<StyledTreeItemProps>(({ rootNode }) => {
  const borderColor = "gray";

  return {
    position: "relative",
    "&:before": {
      pointerEvents: "none",
      content: '""',
      position: "absolute",
      width: 32,
      left: -16,
      top: 12,
      borderBottom:
        // only display if the TreeItem is not root node
        !rootNode ? `1px dashed ${borderColor}` : "none"
    },
    [`& .${treeItemClasses.group}`]: {
      marginLeft: 16,
      paddingLeft: 18,
      borderLeft: `1px dashed ${borderColor}`
    }
  };
});

interface CheckProp {
  label: string;
}

class ItemCheck extends React.Component<CheckProp> {
  constructor(props: CheckProp) {
    super(props)
  }

  render() {
    return (
      <FormControl component="fieldset">
        <FormGroup aria-label="position" row>
          <FormControlLabel
            value="start"
            control={<Switch color="primary" />}
            label={this.props.label}
            labelPlacement="start"
          />
        </FormGroup>
      </FormControl>
    );
  }
}

export default function FileSystemNavigator() {
  return (
    <TreeView
      defaultCollapseIcon={<ExpandMoreIcon />}
      defaultExpandIcon={<ChevronRightIcon />}
      sx={{ height: 240, flexGrow: 1, m: 3 }}
    >
      <StyledTreeItem rootNode nodeId="1" label="Applications">
        <StyledTreeItem nodeId="2" label={ <ItemCheck label="start label" /> } />
        <StyledTreeItem nodeId="3" label="Drive" />
        <StyledTreeItem nodeId="4" label="Inbox">
          <StyledTreeItem nodeId="5" label="One" />
          <StyledTreeItem nodeId="6" label="Two" />
          <StyledTreeItem nodeId="7" label="Three" />
        </StyledTreeItem>
      </StyledTreeItem>
      <StyledTreeItem rootNode nodeId="8" label="Documents">
        <StyledTreeItem nodeId="9" label="OSS" />
        <StyledTreeItem nodeId="10" label="MUI">
          <StyledTreeItem nodeId="11" label="index.js" />
        </StyledTreeItem>
      </StyledTreeItem>
    </TreeView>
  );
}
